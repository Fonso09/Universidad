# sim_5gdl_dh.py
import numpy as np
import pyvista as pv
import vtk
from pathlib import Path

# =========================
# 0) CONFIG - Archivos y unidades
# =========================
FILES = {
    "base_fija":      "base_circular.STL",  # base que no rota
    "base_rotatoria": "cadera_v3.STL", # J1
    "arm1":           "Arm_1.STL",          # J2
    "arm2":           "Arm_2.STL",          # J3
    "arm3":           "Arm_3.STL",          # J4
    "tool_base":      "Base.STL",           # J5 (herramienta / extremo)
}

# Si los STL están en mm pon 0.001 para convertir a metros
UNITS_SCALE = 1

# =========================
# 1) PARÁMETROS DH (rellena con tus valores reales)
# =========================
N = 5  # número de juntas

a = np.zeros(N, dtype=float)
alpha = np.array([-np.pi/2, 0.0, np.pi/2, -np.pi/2, 0.0], dtype=float)
d = np.zeros(N, dtype=float)
theta_offset = np.zeros(N, dtype=float)

joint_type = ['R', 'R', 'R', 'R', 'R']

SLIDER_DEG_RANGES = [
    (-180, 180),
    (-120, 120),
    (-120, 120),
    (-180, 180),
    (-120, 120),
]

# =========================
# 2) FUNCIONES DE MATRICES (MTH)
# =========================

def rot_x(alpha):
    ca, sa = np.cos(alpha), np.sin(alpha)
    return np.array([[1, 0, 0, 0],
                     [0, ca, -sa, 0],
                     [0, sa,  ca, 0],
                     [0, 0,   0,  1]], dtype=float)

def rot_z(theta):
    ct, st = np.cos(theta), np.sin(theta)
    return np.array([[ ct, -st, 0, 0],
                     [ st,  ct, 0, 0],
                     [  0,   0, 1, 0],
                     [  0,   0, 0, 1]], dtype=float)

def trans_x(a):
    T = np.eye(4); T[0,3] = a; return T

def trans_z(d):
    T = np.eye(4); T[2,3] = d; return T

def dh_matrix(a_i, alpha_i, d_i, theta_i):
    return rot_z(theta_i) @ trans_z(d_i) @ trans_x(a_i) @ rot_x(alpha_i)

def numpy_to_vtk_matrix(mat4):
    m = vtk.vtkMatrix4x4()
    for r in range(4):
        for c in range(4):
            m.SetElement(r, c, float(mat4[r, c]))
    return m

def trans_xyz(dx, dy, dz):
    T = np.eye(4); T[0,3]=dx; T[1,3]=dy; T[2,3]=dz; return T

# =========================
# 3) CARGA MALLAS Y PREPARACIÓN
# =========================

def load_and_scale(path):
    mesh = pv.read(path)
    if UNITS_SCALE != 1.0:
        mesh.scale([UNITS_SCALE]*3, inplace=True)
    return mesh

def bottom_center_to_origin(mesh):
    xmin, xmax, ymin, ymax, zmin, zmax = mesh.bounds
    cx, cy = 0.5*(xmin+xmax), 0.5*(ymin+ymax)
    mesh.translate([-cx, -cy, -zmin], inplace=True)
    return mesh

def top_center_to_z0(mesh):
    xmin, xmax, ymin, ymax, zmin, zmax = mesh.bounds
    cx, cy = 0.5*(xmin+xmax), 0.5*(ymin+ymax)
    mesh.translate([-cx, -cy, -zmax], inplace=True)
    return mesh

def extent_z(mesh):
    return mesh.bounds[5]-mesh.bounds[4]

for k, fname in FILES.items():
    if not Path(fname).exists():
        raise FileNotFoundError(f"Archivo '{fname}' no encontrado.")

base_fija = load_and_scale(FILES["base_fija"])
base_rot  = load_and_scale(FILES["base_rotatoria"])
arm1      = load_and_scale(FILES["arm1"])
arm2      = load_and_scale(FILES["arm2"])
arm3      = load_and_scale(FILES["arm3"])
tool_base = load_and_scale(FILES["tool_base"])

top_center_to_z0(base_fija)
bottom_center_to_origin(base_rot)
bottom_center_to_origin(arm1)
bottom_center_to_origin(arm2)
bottom_center_to_origin(arm3)
bottom_center_to_origin(tool_base)

d_est = [
    extent_z(base_rot),
    extent_z(arm1),
    extent_z(arm2),
    extent_z(arm3),
    max(0.02, 0.25*extent_z(tool_base)),
]
for i in range(N):
    d[i] = d_est[i]

T_FIX = {
    "base_rotatoria": np.eye(4),
    "arm1": np.eye(4),
    "arm2": np.eye(4),
    "arm3": np.eye(4),
    "tool_base": np.eye(4),
}

# =========================
# 4) ESCENA
# =========================

plotter = pv.Plotter(window_size=(1200,800))
plotter.add_axes(); plotter.show_grid()
plotter.add_text("Simulación 5GDL (DH + MTH)", font_size=12)
plotter.camera_position="iso"

actors = {}
actors["base_fija"] = plotter.add_mesh(base_fija, name="base_fija")
actors["base_rotatoria"] = plotter.add_mesh(base_rot, name="base_rotatoria")
actors["arm1"] = plotter.add_mesh(arm1, name="arm1")
actors["arm2"] = plotter.add_mesh(arm2, name="arm2")
actors["arm3"] = plotter.add_mesh(arm3, name="arm3")
actors["tool_base"] = plotter.add_mesh(tool_base, name="tool_base")

# =========================
# 5) CINEMÁTICA
# =========================

q = np.zeros(N)

def forward_kinematics_dh(q):
    Ts=[]; T=np.eye(4)
    for i in range(N):
        if joint_type[i]=='R':
            theta_i=theta_offset[i]+q[i]; d_i=d[i]
        else:
            theta_i=theta_offset[i]; d_i=d[i]+q[i]
        A_i=dh_matrix(a[i],alpha[i],d_i,theta_i)
        T=T@A_i; Ts.append(T.copy())
    return Ts

def apply_mesh_poses_from_dh(Ts):
    actors["base_fija"].SetUserMatrix(numpy_to_vtk_matrix(np.eye(4)))
    meshes_info=[("base_rotatoria",d[0]),("arm1",d[1]),("arm2",d[2]),("arm3",d[3]),("tool_base",d[4])]
    for i,(name,di) in enumerate(meshes_info):
        T_i=Ts[i]
        if name=="base_rotatoria": half=extent_z(base_rot)/2
        elif name=="arm1": half=extent_z(arm1)/2
        elif name=="arm2": half=extent_z(arm2)/2
        elif name=="arm3": half=extent_z(arm3)/2
        else: half=extent_z(tool_base)/2
        comp=trans_xyz(0,0,-(di-half))
        actor_mat=T_i@comp@T_FIX[name]
        actors[name].SetUserMatrix(numpy_to_vtk_matrix(actor_mat))
    plotter.render()

Ts0=forward_kinematics_dh(q); apply_mesh_poses_from_dh(Ts0)

# =========================
# 6) SLIDERS
# =========================

def make_slider_callback(i):
    def callback(val_deg):
        if joint_type[i]=='R': q[i]=np.deg2rad(val_deg)
        else: q[i]=val_deg
        Ts=forward_kinematics_dh(q)
        apply_mesh_poses_from_dh(Ts)
    return callback

y0=0.92; dy=0.08
for i in range(N):
    rng=SLIDER_DEG_RANGES[i]
    plotter.add_slider_widget(
        callback=make_slider_callback(i),
        rng=rng, value=0.0,
        title=f"q{i+1}",
        pointa=(0.02,y0-i*dy),
        pointb=(0.45,y0-i*dy),
        style='modern'
        # event_type eliminado
    )

plotter.show()
