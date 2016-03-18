'''
Created on Mar 9, 2015

@author: linhuangjing
'''
import PreprocessingModule
import CNNSegModule
import numpy as np
import cv2
from utils import createFlatView


def ExudateSegment(originImg, ResultPath, presegPath=''):
    presegImg = PreprocessingModule.PreExudate(originImg)
    presegImg = presegImg.astype(np.uint8)
    presegImg = cv2.medianBlur(presegImg, 3)
    
    f_presegImg = createFlatView(presegImg)
 
    for i in xrange(f_presegImg.size):
        if f_presegImg[i]>100:
            f_presegImg[i]=80
        else:
            f_presegImg[i]=0
    #cv2.imwrite(presegPath,presegImg)
    
    CNNSeg = CNNSegModule.ExudateCNNSegNetwork('./ConfExu.txt')
    result=CNNSeg.segment(originImg, presegImg, 128)
    
    #connect two Images
    showImg = np.full((result.shape[0],result.shape[1]*2+3,result.shape[2]), 255, result.dtype)
    showImg[0:originImg.shape[0],0:originImg.shape[1],:]=originImg[:,:,:]
    showImg[0:result.shape[0],originImg.shape[1]+3:originImg.shape[1]+result.shape[1]+3,:]= result[:,:,:]
    
    cv2.imwrite(ResultPath,showImg)
    
def HamorSegment(originImg, ResultPath, presegPath=''):
#     cv2.imwrite("./temp_ori.jpg",originImg)
    featureImg,presegImg,tmpImg = PreprocessingModule.PreHamor2(originImg)
#     presegImg = presegImg.astype(np.uint8)
#     presegImg = cv2.medianBlur(presegImg, 3)
#     
#     f_presegImg = createFlatView(presegImg)
#  
#     for i in xrange(f_presegImg.size):
#         if f_presegImg[i]>100:
#             f_presegImg[i]=80
#         else:
#             f_presegImg[i]=0

###Preprocess by Jinpeng
    
#     originImg1 = cv2.imread("/home/linhuangjing/Desktop/Retina_Data/Group-1/DR/DR_102.jpg")
    
    
    cv2.imwrite("./temp_ori.jpg",originImg)
    originImg2 = cv2.imread("./temp_ori.jpg")
    
#     cv2.imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/strange.jpg",originImg1-originImg2)
    
    import JPModule
    JPModule.Preprocess("./temp_ori.jpg","./temp_jppre.jpg")
    JPImg=cv2.imread("./temp_jppre.jpg")
    b,g,r = cv2.split(JPImg)
    
    cv2.imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/MyPre_102.jpg",presegImg)
    cv2.imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/JPPre_102.jpg",JPImg)
    
    #Mask two images
    f_g = createFlatView(g)
    f_presegImg = createFlatView(presegImg)
    for i in xrange(f_presegImg.size):
        if f_presegImg[i]>f_g[i]:
            f_presegImg[i] = f_g[i]    
    
    
    cv2.imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/Pre_102.jpg",presegImg)
    

    preImg3 = np.zeros(originImg.shape,presegImg.dtype)
    cv2.merge((presegImg,presegImg,presegImg),preImg3)
    showTmpImg = np.full((originImg.shape[0],originImg.shape[1]*2+3,originImg.shape[2]), 255, tmpImg.dtype)
    showTmpImg[0:preImg3.shape[0],0:preImg3.shape[1],:]=preImg3[:,:,:]
    showTmpImg[0:tmpImg.shape[0],preImg3.shape[1]+3:preImg3.shape[1]+tmpImg.shape[1]+3,:]= tmpImg[:,:,:]
    cv2.imwrite(presegPath,showTmpImg)
    
    
    featureImg = featureImg.clip(0,255)
    cv2.imwrite("/home/linhuangjing/Desktop/featureImg.jpg",featureImg)
    
    CNNSeg = CNNSegModule.HamorCNNSegNetwork('./ConfHamor.txt')
    result=CNNSeg.segment(featureImg, presegImg, 1)
    
    #connect two Images
#     showImg = np.full((result.shape[0],result.shape[1]*2+3,result.shape[2]), 255, result.dtype)
#     showImg[0:originImg.shape[0],0:originImg.shape[1],:]=originImg[:,:,:]
#     showImg[0:result.shape[0],originImg.shape[1]+3:originImg.shape[1]+result.shape[1]+3,:]= result[:,:,:]

    result=result.astype(float)
    b,g,r = cv2.split(result)
    a = np.zeros(g.shape,g.dtype)
    
    f_g = createFlatView(g)
    f_a = createFlatView(a)
    f_r = createFlatView(r)
    f_b = createFlatView(b)
    for i in xrange(f_g.size):
        if f_g[i]>20 or f_r[i]>20 or f_b[i]>20:
            f_a[i]=255
    
    showImg = np.zeros((result.shape[0],result.shape[1],4),g.dtype)
    cv2.merge((b,g,r,a),showImg)
    
    cv2.imwrite(ResultPath,showImg)
    
    
    
    
    
    
        