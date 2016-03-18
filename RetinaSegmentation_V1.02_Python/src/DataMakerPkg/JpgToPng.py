'''
Created on Apr 19, 2015

@author: linhuangjing
'''

import os
import cv2
import numpy as np


def createFlatView(array):
    '''Return 1D view of the array, to accerelate the calculation'''
    flatView = array.view()
    flatView.shape = array.size
    return flatView


oPath = "/home/linhuangjing/Desktop/HamorResults/DR Hamor_V7/DR_"
rPath= "/home/linhuangjing/Desktop/Results/V7/DR_"

for i in xrange(101,181):
    originPath = oPath+str(i)+'_result.jpg'
    ResultPath = rPath+str(i)+'_result.png'

    img = cv2.imread(originPath);

    img=img.astype(float)
    b,g,r = cv2.split(img)
    
    a = np.zeros(g.shape,g.dtype)
    
    f_g = createFlatView(g)
    f_a = createFlatView(a)
    f_r = createFlatView(r)
    f_b = createFlatView(b)
    for i in xrange(f_g.size):
        if f_g[i]>20 or f_r[i]>20 or f_b[i]>20:
            f_a[i]=255
    
    PNGImg = np.zeros((img.shape[0],img.shape[1],4),g.dtype)
    cv2.merge((b,g,r,a),PNGImg)
    cv2.imwrite(ResultPath,PNGImg)

print "Completed!"