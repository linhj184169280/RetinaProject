'''
Created on Mar 6, 2015

@author: linhuangjing
'''
import numpy as np
import cv2

def createFlatView(array):
    '''Return 1D view of the array, to accerelate the calculation'''
    flatView = array.view()
    flatView.shape = array.size
    return flatView

def GetPixel(img,row,col):
    '''GetPixel(img, row, col)
       Return pixel in [row, col]. if outside the img, it will return 0. 
    '''
    if row>=0 and row<img.shape[0] and col>=0 and col<img.shape[1]:
        return img[row ,col]
    else:
        return 0

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

def avglight(img, width, under=25):
    img = img.astype(np.uint8)
    img = cv2.medianBlur(img,3)
    
    dst = np.zeros(img.shape, img.dtype)

    
    for y in xrange(img.shape[0]):
        for x in xrange(img.shape[1]):
            tot=0; totnum=0;
            tempyy = y-width; tempxx = x-width;
            tempyy2= y+width; tempxx2= x+width;
            if tempyy<0:
                tempyy=0
            if tempxx<0:
                tempxx=0
            if tempyy2>img.shape[0]:
                tempyy2=img.shape[0]
            if tempxx2>img.shape[1]:
                tempxx2=img.shape[1]
            if img[y,x]<15:
                dst[y,x]=img[y,x]
                continue
            for yy in xrange(tempyy,tempyy2):
                for xx in xrange(tempxx,tempxx2):
                    if img[yy, xx]<under:
                        continue
                    tot = tot+img[yy, xx]
                    totnum = totnum+1
            if totnum!=0:
                dst[y, x] = img[y,x]+120-tot/totnum
    return dst
            
            
            
            
            
    
    
    