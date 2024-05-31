import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
def f1(z,h):
    return h-z

def f2(z,h):
    return np.sqrt(1-(2*z/h-1)*(2*z/h-1))*h/2

def gen_points(num_points, height, func):
    # 高さ方向の座標 z を生成
    z = height * np.random.rand(num_points)
    fz = func(z, height) * func(z, height)
    r = np.sqrt(fz * fz * np.random.rand(num_points))
    theta = 2 * np.pi * np.random.rand(num_points)
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    return x, y, z

def sphere_points(num_points,radius):
    cosT=-2*np.random.rand(num_points)+1
    sinT=np.sqrt(1-cosT*cosT)
    phi=2*np.pi*np.random.rand(num_points)
    r=radius*np.power(np.random.rand(),1/3)
    x=r*sinT*np.cos(phi)
    y=r*sinT*np.sin(phi)
    z=r*cosT

    return x,y,z


# 使用例
num_points = 1000
height = 10
radius = 5

x, y, z = gen_points(num_points, height, f1)
# x,y,z=sphere_points(num_points,radius)
# 3Dプロットの作成
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.scatter(x, y, z, c='b', marker='.')

# 軸ラベルの設定
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# プロットの表示
plt.show()
