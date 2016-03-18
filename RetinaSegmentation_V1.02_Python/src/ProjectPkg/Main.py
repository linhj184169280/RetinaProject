import os
import cv2
import numpy

import utils
from utils import createFlatView

import numpy as np
import CNNRetinaSegFilter as SegFilter

import time

oPath = "/home/linhuangjing/Desktop/Retina_Data/Group-0/DR raw/DR-"
rPath= "/home/linhuangjing/Desktop/Retina_Data/Group-0/results/DR-"
pPath = "/home/linhuangjing/Desktop/Retina_Data/Group-0/results/Preseg_"
# OriginPath = "/home/linhuangjing/Desktop/Retina_Data/DR raw/DR-1.JPG"
# ResultPath= "/home/linhuangjing/Desktop/Retina_Data/results/DR-1_result.jpg"
# presegPath = "/home/linhuangjing/Desktop/Retina_Data/results/Preseg_1.jpg"

file = open("/home/linhuangjing/Desktop/Retina_Data/Group-0/results/DR-timeCompute.txt",'w')

for i in xrange(2,3):
    originPath = oPath+str(i)+'.jpg'
    ResultPath = rPath+str(i)+'_result.jpg'
    presegPath = pPath+str(i)+'.jpg'
    img = cv2.imread(originPath);
    print img.shape
    file.write("DR"+str(i)+': shape='+str(img.shape))
    time1 = time.time()
    
    SegFilter.HamorSegment(img, ResultPath, presegPath)
    
    time2 = time.time()
    alltime=time2-time1
    file.write("  consume time="+str(alltime)+'\n')
print "Completed!"

