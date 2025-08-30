import numpy as np
import pyvista as pv
from pathlib import Path
import vtk

# =========================
# ARCHIVOS y UNIDADES
# =========================
FILES = {
    "base_fija": "base_fija.STL",  # base fija (no se mueve)
    "J0": "J0.STL",                # eslabón 1
    "J1": "J1.STL",                # eslabón 2
    "J2": "J2.STL",                # eslabón 3
    "J3": "J3.STL",                # eslabón 4
    "J4": "J4.STL",                # eslabón 5 (último)
}
UNITS_SCALE = 0.001  # tus STLs estaban en mm -> los pasamos a m

# =========================
# LONGITUDES (m) según medidas que diste
# =========================
L1 = 0.0435
L2 = 0.140   # (si no se usa en tabla no importa pero lo dejo por si acaso)
L3 = 0.105
L4 = 0.0264

# =========================
# Funciones auxiliares
# =========================
def dh_matrix(theta, d, alpha, a):
    """Convención DH estándar: (theta, d, alpha, a) -> 4x4 numpy matrix"""
    ct, st = np.cos(theta), np.sin(theta)
    ca, sa = np.cos(alpha), np.sin(alpha)
    return np.array([
        [ ct, -st*ca,  st*sa, a*ct],
        [ st,  ct*ca, -ct*sa, a*st],
        [  0,     sa,     ca,    d],
        [  0,      0,      0,    1]
    ], dtype=float)

def numpy_to_vtk_matrix(mat):
    """Convierte una matriz numpy 4x4 en vtk.vtkMatrix4x4"""
    vtk_mat = vtk.vtkMatrix4x4()
    for i in range(4):
        for j in range(4):
            vtk_mat.SetElement(i, j, float(mat[i, j]))
    return vtk_mat

# =========================
# CARGA DE MESHES y NORMALIZACIÓN
# =========================
meshes = {}
for k, fname in FILES.items():
    mesh = pv.read(Path(fname))
    # escala de mm -> m
    mesh.points *= UNITS_SCALE
    # centramos el mesh en su origin local para que al aplicar la MTH quede "pegado"
    # usamos bounding-box center como corrección rápida:
    center = mesh.center  # center devuelve (x,y,z) del centro del bounding box
    mesh.points -= np.array(center)
    meshes[k] = mesh

# =========================
# PLOTTER y ACTORES
# =========================
plotter = pv.Plotter(window_size=(1200, 800))
plotter.add_axes()
plotter.show_grid()
plotter.add_text("Simulación Rob 5GDL - usando tabla DH provista", font_size=12)
plotter.camera_position = "iso"

actors = {}
for k, m in meshes.items():
    # Añadimos malla; guardamos el "actor" devuelto para SetUserMatrix
    actors[k] = plotter.add_mesh(m, color="lightsteelblue", show_scalar_bar=False)

# esfera para TCP (visual)
tcp_sphere = pv.Sphere(radius=0.01)  # tamaño visual; ajustar si es necesario
tcp_actor = plotter.add_mesh(tcp_sphere, color="red")

# =========================
# ESTADO DE LAS ARTICULACIONES (grados)
# =========================
q_vals_deg = [0.0, 0.0, 0.0, 0.0, 0.0]  # q1..q5 (en grados)

# =========================
# Tabla DH (según tu entrada):
# [
# n,thetan,dn,an,rn;
# 1,q1+0,L1,pi/2,0;
# 2,q2+0,0,0,0;
# 3,q3+pi/2,0,pi/2,L3;
# 4,q4+0,L4,-pi/2,0;
# 5,q5+0,0,0,0;
# ]
# Interpretación: columnas -> (theta, d, alpha, a)
# =========================
def compute_transforms_from_DH(q_deg_list):
    """Devuelve lista de T_cumulativas: [T0, T1, T2, T3, T4, T5] (numpy 4x4 each)"""
    q1 = np.deg2rad(q_deg_list[0])
    q2 = np.deg2rad(q_deg_list[1])
    q3 = np.deg2rad(q_deg_list[2])
    q4 = np.deg2rad(q_deg_list[3])
    q5 = np.deg2rad(q_deg_list[4])

    # según tu tabla
    A1 = dh_matrix(q1 + 0.0, L1,    np.pi/2, 0.0)
    A2 = dh_matrix(q2 + 0.0, 0.0,   0.0,     0.0)
    A3 = dh_matrix(q3 + np.pi/2, 0.0, np.pi/2, L3)
    A4 = dh_matrix(q4 + 0.0, L4,   -np.pi/2, 0.0)
    A5 = dh_matrix(q5 + 0.0, 0.0,  0.0,      0.0)

    Ts = []
    Tcum = np.eye(4)
    Ts.append(Tcum.copy())        # T0
    for A in (A1, A2, A3, A4, A5):
        Tcum = Tcum @ A
        Ts.append(Tcum.copy())

    return Ts  # length 6

# =========================
# FUNCION DE ACTUALIZACION (aplica MTH a cada actor)
# =========================
def update_scene():
    Ts = compute_transforms_from_DH(q_vals_deg)

    # base fija: si quieres mantener la base en su sitio (ya centrada), no la movemos.
    # si tu base tiene que colocarse en T0, podrías hacer actors["base_fija"].SetUserMatrix(...)
    # En este script dejamos la base tal cual (centrada) porque normalmente la base ya está en el origen CAD.

    # Ahora colocamos J0..J4 con las transformadas acumuladas:
    # Observa: Ts[1] coloca J0 (primer eslabón), Ts[2] coloca J1, ...
    mapping = [("J0", 1), ("J1", 2), ("J2", 3), ("J3", 4), ("J4", 5)]
    for name, idx in mapping:
        T = Ts[idx]
        vtkT = numpy_to_vtk_matrix(T)
        actors[name].SetUserMatrix(vtkT)

    # actualizar TCP: tomar posición del origen del último marco (T5 * [0,0,0,1])
    T5 = Ts[5]
    tcp_pos = T5[:3, 3]  # x,y,z en metros
    # movemos la esfera: construimos matriz de traslación
    T_tcp = np.eye(4)
    T_tcp[:3, 3] = tcp_pos
    tcp_actor.SetUserMatrix(numpy_to_vtk_matrix(T_tcp))

    plotter.render()

# =========================
# SLIDERS (separados a la derecha, vertical)
# =========================
def make_callback(i):
    def cb(value):
        q_vals_deg[i] = float(value)
        update_scene()
    return cb

# posiciones de slider (pointa, pointb) en coordenadas normalizadas ventana
slider_positions = [
    ((0.86, 0.85), (0.98, 0.85)),  # q1 (arriba)
    ((0.86, 0.75), (0.98, 0.75)),  # q2
    ((0.86, 0.65), (0.98, 0.65)),  # q3
    ((0.86, 0.55), (0.98, 0.55)),  # q4
    ((0.86, 0.45), (0.98, 0.45)),  # q5 (abajo)
]

# rangos: los puse amplios; ajuste si quieres límites diferentes
ranges = [
    (-180, 180),
    (-180, 180),
    (-180, 180),
    (-180, 180),
    (-180, 180),
]

for i in range(5):
    p_a, p_b = slider_positions[i]
    plotter.add_slider_widget(
        callback=make_callback(i),
        rng=[ranges[i][0], ranges[i][1]],
        value=float(q_vals_deg[i]),
        pointa=p_a,
        pointb=p_b,
        title=f"q{i+1}"
    )

# =========================
# Inicializar y mostrar
# =========================
update_scene()
plotter.show()
