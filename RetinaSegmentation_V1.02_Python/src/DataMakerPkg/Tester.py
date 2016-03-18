# Apparence Tester
# import os
# import cv2
# import numpy as np
# 
# 
# OriginPath = "/home/linhuangjing/Desktop/Retina_Data/Group-1/results/DR-1_t.png"
# ResultPath= "/home/linhuangjing/Desktop/Retina_Data/Group-1/results/DR-1_t2.png"
# 
# img = cv2.imread(OriginPath);
# b,g,r = cv2.split(img)
# a = np.zeros(r.shape,r.dtype)
# 
# tshape = (img.shape[0],img.shape[1],4)
# tmpImg = np.zeros(tshape,img.dtype)
# tmpImg=cv2.cvtColor(img,cv2.COLOR_BGR2BGRA)
# tmpImg[:,:,3]=255
# 
# cv2.imwrite(ResultPath,tmpImg)
# 
# print 'complete!'

import numpy as np
import cv2
import sys
import time
from functools import partial

from numbapro import vectorize
from Canvas import Window
from numba.npyufunc.decorators import guvectorize

from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool
import multiprocessing
import pathos.pp_map as mp

import itertools

originPath = '/home/linhuangjing/Desktop/Retina_Data/Group-1/DRF/DRF_102.jpg'
labelPath = '/home/linhuangjing/Desktop/Retina_Data/Group-1/Prepro_V1.01 80/Prepro Image(Labelled)/dst_102.jpg'


def GetPatchesIndexList(presegImg):
    '''
    '''
    indexesList = []
        #Judge if the presegImg is a gray-scale image
    if presegImg.ndim==3:
        b,g,r = cv2.split(presegImg)
    else:
        r = presegImg
    iter = r.flat
    for i in xrange(r.size):
        value = iter.next()
        if value>30:
            index = iter.coords
            indexesList.append(np.asarray(index))
    return np.asarray(indexesList)

def GetPatch(originImg, row, col, windowSize):
    '''
    '''
    radius = windowSize/2
        
    px = col-radius
    py = row-radius
    qx = col+radius
    qy = row+radius
    if px<0:
        px=0
    if py<0:
        py=0
    if qx>=originImg.shape[1]:
        qx=originImg.shape[1]-1
    if qy>=originImg.shape[0]:
        qy=originImg.shape[0]-1
    patchImage = originImg[py:qy, px:qx, :]
    return patchImage.astype(np.float32)


# @guvectorize(['void(float32[:,:,:],int32[:,:],int32[:],float32[:,:,:,:])'],'(m,n,c),(k,d),(w)->(k,w,w,c)')
def GetBatch_(originImg, windowSize, SubIndexesList):
    m,n,c = originImg.shape
    k,d = SubIndexesList.shape
    w = windowSize
    t=0
    Batch=[]
    for i in xrange(k):
#         Batch[i,:,:,:] = GetPatch(originImg, SubIndexesList[i,0], SubIndexesList[i,1], w)    
        patch = GetPatch(originImg, SubIndexesList[i,0], SubIndexesList[i,1], w)    
        Batch.append(patch)
        t=t+1
        if t>indexesList.shape[0]/8:
            print 'predicting...: '+str(float(i*100)/indexesList.shape[0])+'%'
            t=0
#     return Batch
def GetBatch(originImg_windowSize_SubIndexesList):
    return GetBatch_(*originImg_windowSize_SubIndexesList)
    
originImg = cv2.imread(originPath)
originImg = originImg.astype(np.float32)
labelImg = cv2.imread(labelPath)

indexesList = GetPatchesIndexList(labelImg)
# win_z = originImg.shape[0]/12
win_z = 100
win_z = win_z/2*2
windowSize = win_z

batchSize = 2048
i_Size = indexesList.shape[0]/batchSize
SubIndexesListSet = np.zeros((i_Size, batchSize, 2),indexesList.dtype)

partial_GetBatch = partial(GetBatch, originImg, windowSize)

time1 = time.time()
pool = Pool()
for i in xrange(i_Size):
    SubIndexesListSet[i] = indexesList[i*batchSize:(i+1)*batchSize]
#     partial_GetBatch(SubIndexesListSet[i])

pool.map(GetBatch, itertools.izip(itertools.repeat(originImg), itertools.repeat(windowSize),SubIndexesListSet))

    # Batch = np.zeros((10000,win_z,win_z,3),originImg.dtype)
#     batch=[]
#     GetBatch(originImg, SubIndexesListSet, windowSize)
# partial_GetBatch(SubIndexesListSet[i])


 
pool.close()
pool.join()
######Alone
    
time2 = time.time()
alltime=time2-time1   
print "all time = ",alltime
# i=0
# ba_j=0
# batchsize=128
# flag=0
# t=0
# while(i<indexesList.shape[0]):
#     index = indexesList[i]
#     patch = GetPatch(originImg, index[0], index[1], 100)
#     batch.append(patch)
#     ba_j=ba_j+1;
#     i=i+1
#     t=t+1
#     if t>indexesList.shape[0]/8:
#         print 'predicting...: '+str(float(i*100)/indexesList.shape[0])+'%'
#         t=0
#     #process $batchsize patches a time
#     if ba_j==batchsize:
#         #predictions.append(preds)
#         if(flag==0):
#             flag=1
#         ba_j=0
#         batch = []       
#     


print 'Complete!'




