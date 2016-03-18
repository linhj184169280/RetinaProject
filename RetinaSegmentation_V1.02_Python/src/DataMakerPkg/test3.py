'''
Created on Apr 15, 2015

@author: linhuangjing
'''
import JPModule

# #libtest = cdll.LoadLibrary(os.getcwd()+'/JPModule.so')
# src="/home/linhuangjing/Desktop/Retina_Data/Group-1/DR/DR_101.jpg"
# #dst="/home/linhuangjing/Desktop/Retina_Data/Group-1/results/JinPengPre_101.jpg"
# dst="./JinPengPre_101.jpg"
# 
# JPModule.Preprocess(src,dst)

import cv2
import numpy as np

img = cv2.imread("/home/linhuangjing/Desktop/ExperTmp/b1.jpg")


print "Complete!"