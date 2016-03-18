'''
Created on Mar 6, 2015

@author: linhuangjing
'''
import cv2
import numpy as np
import utils
from utils import createFlatView
import utils


def PreExudate(src):
    '''Preprocess image of exudates by estimate the Background via a Median Filter,
       Which is used for exudates preprocessing.
    '''
    # InImage=cv2.cvtColor(src,cv2.cv.CV_BGR2GRAY)
    print 'Preprocessing...'
    img=src.astype(float)
    b,g,r = cv2.split(img)
    InImage = np.zeros(g.shape,g.dtype)
    cv2.addWeighted(b,0.333333,r,0.333333,0,InImage)
    cv2.addWeighted(g,0.333333,InImage,1.0,0,InImage)
#     cv2.addWeighted(b,0.5,r,0.5,0,InImage)
    
    wsize=img.shape[0]/30
    if wsize%2 is 0:
        wsize=wsize+1
    
    InImage = InImage.astype(np.uint8)
    bgEst = cv2.medianBlur(InImage,wsize)
    
    
    bgMask = np.zeros(bgEst.shape,bgEst.dtype)
    bgMask2 = np.zeros(bgEst.shape,bgEst.dtype)
    
    
    f_InImage = createFlatView(InImage)
    f_bgEst = createFlatView(bgEst)
    f_bgMask = createFlatView(bgMask)
    f_bgMask2 = createFlatView(bgMask2)
    
    for i in xrange(f_bgMask.size):
        if f_InImage[i]>f_bgEst[i]:
            f_bgMask[i] = f_InImage[i]
            f_bgMask2[i] = f_bgEst[i]
        else:
            f_bgMask[i] = f_bgEst[i]
            f_bgMask2[i] = f_InImage[i]
    
   
    im1 = bgMask-bgMask2
    im2 = g-bgEst
    dst = np.zeros(im2.shape,im2.dtype)
     
    f_im1 = createFlatView(im1)
    f_im2 = createFlatView(im2)
    f_dst = createFlatView(dst)
     
    for i in xrange(f_im1.size):
        if f_im1[i]<8:
            f_im1[i] = 0
        if f_im2[i]<6:
            f_im2[i] = 0
        if f_im1[i]<f_im2[i]:
            f_dst[i] = f_im1[i]
        else:
            f_dst[i] = f_im2[i]
        if f_dst[i]>5:
            f_dst[i] = 255
        else:
            f_dst[i] = 0
    return dst


def PreHamor(src):
    '''Preprocess image of Hamorrhage by morphology. 
    '''
    print 'Preprocessing...'
    img=src.astype(float)
    b,g,r = cv2.split(img)
    
    kernel = cv2.getStructuringElement(cv2.MORPH_CROSS,(20, 20)) 
#    eroded = cv2.erode(g,kernel) 
#    dilated = cv2.dilate(eroded,kernel)

    blackhat = cv2.morphologyEx(g, cv2.MORPH_BLACKHAT, kernel) 
    
    tmp = g-blackhat*5
    dst = g-blackhat*5
#     blackhat = cv2.morphologyEx(dst, cv2.MORPH_BLACKHAT, kernel) 
#     tmp = g-blackhat

    
#     wsize=img.shape[0]/30
#     if wsize%2 is 0:
#         wsize=wsize+1
#     
#     tmp=tmp.clip(0,255)
#     tmp1 = tmp.astype(np.uint8)
#     bgEst = cv2.medianBlur(tmp1,wsize)
#     
#     dst=(bgEst-tmp).clip(0,255)
#     
#     f_dst = createFlatView(dst)
#     for i in xrange(f_dst.size):
#         if f_dst[i]<20:
#             f_dst[i]=0
#     dst=dst*5
    dst.clip(0,255)
    f_dst = createFlatView(dst)
    f_src = createFlatView(src)
    #The background area must be removed
    for i in xrange(f_dst.size):
        if f_dst[i]<15 and (f_src[3*i]>10 or f_src[3*i+1]>10 or f_src[3*i+2]>10):
            f_dst[i]=80
        else:
            f_dst[i]=0
#     beta=10
#     alpha=-0.05
#     dst = 255/(1+np.exp(-(tmp-beta)/alpha))
    dst = dst.astype(np.uint8)
    dst = cv2.medianBlur(dst,3)  
    return dst

def PreHamor2(src):
    '''Preprocess image of Hamorrhage by morphology. 
    '''
    print 'Preprocessing...'
    img=src.astype(float)
    b,g,r = cv2.split(img)
    
    kernel = cv2.getStructuringElement(cv2.MORPH_CROSS,(20, 20)) 
#    eroded = cv2.erode(g,kernel) 
#    dilated = cv2.dilate(eroded,kernel)

    blackhat = cv2.morphologyEx(g, cv2.MORPH_BLACKHAT, kernel) 
    
    tmp = g-blackhat*5
    dst = g-blackhat*5
 
    tmp.clip(0,255)
    dst.clip(0,255)
    f_dst = createFlatView(dst)
    f_src = createFlatView(src)
    #The background area must be removed
    for i in xrange(f_dst.size):
        if f_dst[i]<30 and (f_src[3*i]>10 or f_src[3*i+1]>10 or f_src[3*i+2]>10):
            f_dst[i]=80
        else:
            f_dst[i]=0



#     dst = dst.astype(np.uint8)
#     dst = cv2.medianBlur(dst,3)
#     
    
#     tmp_xx=cv2.filter2D(tmp,-1, np.array([[0,0,0],[0,1,-1],[0,-1,1]]))
#     beta=0
#     alpha=30
#     tmp_xx = 255/(1+np.exp(-(tmp_xx-beta)/alpha))
#     tmp_xy=cv2.filter2D(tmp,-1, np.array([[0,0,0],[0,1,-1],[0,-1,1]]))
#     beta=0
#     alpha=30
#     tmp_xy = 255/(1+np.exp(-(tmp_xy-beta)/alpha))
#     tmp_yy=cv2.filter2D(tmp,-1, np.array([[0,1,0],[0,-2,0],[0,1,0]]))
#     beta=0
#     alpha=30
#     tmp_yy = 255/(1+np.exp(-(tmp_yy-beta)/alpha))
    
    featureImg = np.zeros(img.shape,img.dtype)
    cv2.merge((tmp,g,tmp),featureImg)
    tmpImg = np.zeros(img.shape,img.dtype)
    cv2.merge((tmp,tmp,tmp),tmpImg)
    
    return featureImg,dst,tmpImg



if __name__=='__main__':
    for i in xrange(101,103):
        
    
        oPath = "/home/linhuangjing/Desktop/Retina_Data/Group-1/DR/DR_"
        rPath = "/home/linhuangjing/Desktop/Retina_Data/Group-1/results/Result_"
        
        originPath = oPath+str(i)+'.jpg'
        img = cv2.imread(originPath);
        
#         featureImg,dst,tmpImg = PreHamor2(img)
#         
#         dstPath = rPath+'DRF_'+str(i)+'.jpg'
#         cv2.imwrite(dstPath, featureImg)
#         
#         dstPath = rPath+'dst_'+str(i)+'.jpg'
#         cv2.imwrite(dstPath, dst)
        dst = PreExudate(img)
        dstPath = rPath+'dst_'+str(i)+'.jpg'
        cv2.imwrite(dstPath, dst)
        
    
    print "Complete!"
    


