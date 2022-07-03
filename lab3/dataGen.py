import scipy.sparse as ss
import numpy as np

M = 2000
N = 200000

vec = np.random.randint(-32,31,N)
# vec = np.random.rand(N)

f = open('data.txt','w')
for x in vec:
    f.write(str(x))
    f.write(' ')
f.write('\n')

mat = ss.random(M,N,density=0.01,format='coo',dtype='int8')
first = True
for i,j,k in zip(mat.row,mat.col,mat.data):
    if first:
        first = False
    else:
        f.write('\n')
    f.write(str(i))
    f.write(' ')
    f.write(str(j))
    f.write(' ')
    f.write(str(k))
f.close()

res = mat*vec
f = open('result.txt','w')
for ans in res:
    f.write(str(ans))
    f.write('\n')
