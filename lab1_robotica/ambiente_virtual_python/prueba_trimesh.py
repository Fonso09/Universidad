import trimesh
import numpy as np

# Cargar un eslabón desde STL
link1 = trimesh.load("base_circular.stl")

# Ejemplo de MTH (rotación + traslación)
T = np.array([
    [1, 0, 0, 0.1],  # rotaciones y traslaciones
    [0, 1, 0, 0.2],
    [0, 0, 1, 0.3],
    [0, 0, 0, 1]
])

# Aplicar la transformación
link1.apply_transform(T)

# Visualizar
scene = trimesh.Scene([link1])
scene.show()