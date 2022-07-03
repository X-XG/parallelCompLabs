import numpy as np
import scipy.sparse as sp
import random
import math
from scipy.sparse.csgraph import dijkstra
from scipy.sparse import csr_matrix


num = 1000   # 顶点数
edge = 10 # 每个点的出度

ij = np.random.randint(num*100, size=(2, num))
x = ij[0]
y = ij[1]

map = sp.lil_matrix((num,num),dtype='uint32')

for i in range(num):
    for k in range(edge):
        j = random.randint(0,num-1)
        dist = random.randint(0,num-1)
        map[i,j]=dist
        map[j,i]=dist

f = open('map.txt','w')
map = map.tocoo()
for i,j,d in zip(map.row, map.col, map.data):
    f.write(str(i))
    f.write('\t')
    f.write(str(j))
    f.write('\t')
    f.write(str(d))
    f.write('\n')
f.close()

f = open('result.txt','w')
res = dijkstra(map, return_predecessors=True, indices=0)
for dist in res[0]:   
    if(dist ==float('inf')):
        f.write('X')
        f.write('\n')
        continue
    f.write(str(int(dist)))
    f.write('\n')
