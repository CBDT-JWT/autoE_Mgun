import math
from scipy.interpolate import interp1d  
import numpy as np
import pandas as pd  

from scipy.optimize import curve_fit  
import matplotlib.pyplot as plt  

bullet_len_mm = 22
bullet_rad_mm = 8/2
class Coil:  
    def __init__(self, location, length, N, innerRadius, outerRadius):  
        self.length = length      #线圈长度
        self.innerRadius = innerRadius  #线圈内半径
        self.outerRadius = outerRadius  #线圈外半径
        self.N = N                      #线圈匝数
        self.μ0 = 4 * math.pi * 1e-7  # 真空磁导率   亨利每米 
    def HFieldCoil3(self, current, z):   #输入位置 计算磁场强度！
        L=self.length
        a=L/2
        r2=self.outerRadius
        r1=self.innerRadius
        R=r2-r1
        def logarithm(pos):  
            return pos * math.log((math.sqrt(pos ** 2 + r2 ** 2) + r2) /  
                                  (math.sqrt(pos ** 2 + r1 ** 2) + r1))  
          
        return ( self.N*current/(2*L*R) ) * (logarithm(z+a)-logarithm(z-a)) 
coil = Coil(0, 23.4*1e-3, 200, 0.5*12.4*1e-3, 0.5*40*1e-3)  
current = 200  # 电流为200A  
plt.figure(figsize=(10,10))
xx=[ pos for pos in np.arange(-30,30,0.1)]
yy=[ coil.HFieldCoil3( current, x*1e-3)   for x in xx]
Fyy = []
for i in range (0,len(xx)):
    Fyy.append((coil.HFieldCoil3( current, (xx[i]+bullet_len_mm/2)*1e-3)
                -coil.HFieldCoil3( current, (xx[i]-bullet_len_mm/2)*1e-3))
               *(bullet_rad_mm*1e-3)**2*math.pi)
plt.subplot(1,2,1)
plt.plot(xx, yy, color='red', label=' H field')  
plt.xlabel('x (mm)')  
plt.ylabel('H (A/m)')  
plt.legend()  
plt.subplot(1,2,2)
plt.plot(xx, Fyy, color='blue', label=' force')
plt.xlabel('x (mm)')
plt.ylabel('F (N)')
plt.legend()
plt.show()
