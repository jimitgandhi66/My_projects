


import numpy as np
from numpy import random
from numpy.random import normal

unocc = np.loadtxt('unoccu.dat', delimiter=' ')
ind = np.random.randint(0, unocc.shape[0],1)
coord = unocc[ind]
dist_x = np.random.normal(coord[0],20,100)
dist_y = np.random.normal(coord[1],20,100)
dist=np.column_stack((dist_x,dist_y))
