'''
Created on Mar 13, 2015

@author: linhuangjing
'''
import cv2
import numpy as np

originPath = "/home/linhuangjing/Desktop/Retina_Data/Group-1/DR/DR_105.jpg"
outPath = "/home/linhuangjing/Desktop/ExperTmp/"

img = cv2.imread(originPath);
b,g,r=cv2.split(img)

InImage = np.zeros(g.shape,g.dtype)
cv2.addWeighted(b,0.333333,r,0.333333,0,InImage)
cv2.addWeighted(g,0.333333,InImage,1.0,0,InImage)

cv2.imwrite(outPath+"b.jpg", b)
cv2.imwrite(outPath+"g.jpg", g)
cv2.imwrite(outPath+"r.jpg", r)

wsize=img.shape[0]/30
if wsize%2 is 0:
    wsize=wsize+1
    
bgEst = cv2.medianBlur(InImage,wsize)
b=b.astype(np.float32)
g=g.astype(np.float32)
r=r.astype(np.float32)
bgEst=bgEst.astype(np.float32)
cv2.imwrite(outPath+"InImage.jpg", InImage)
cv2.imwrite(outPath+"bgEst.jpg", bgEst)

cv2.imwrite(outPath+"tmp.jpg", ((bgEst-g)).clip(0,100))


