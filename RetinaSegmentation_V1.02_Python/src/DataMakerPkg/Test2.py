'''
Created on Apr 14, 2015

@author: linhuangjing
'''
import numpy as np
import time
from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool
import multiprocessing


def func(total):
    for i in xrange(10000000):
        total=total+i
#     i=0
#     while i<1000000:
#         total=total+i
#         i=i+1     
    return total

time1 = time.time()
pool = Pool(8)

# for i in xrange(100):
#     total=0
#     pool.apply_async(func, (total,))
arr=np.asarray([1,2,3,4,5])
results = pool.map(func, xrange(50))
# results = map(func, range(50))
#     func(total)

pool.close()
pool.join()
print results

time2 = time.time()
alltime=time2-time1


print "all time = ",alltime