'''
Created on Mar 27, 2015

@author: linhuangjing
'''
import cv2
import numpy as np
import os
import random

class DataMakerBase:
    '''This is the base class of Every DateMaker.
       The concrete method is Apply() which is defined in its subclasses.
    '''
    def __init__(self):
        '''Initial the window proportion size.
        '''      
        self._win_proportion = 12
        

    def _GetPatch(self, originImg, row, col, windowShape):
        '''Get a windowShape Patch from originImg in [row,col]
           originImg is a "ndarray" type image.
           row & col is "int" type coordinates.
           windowShape is a window like [height,width,3]
        ''' 
        radius = windowShape[0]/2
        
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
        
        
    def SetWinProportion(self, winProportion):
        '''Set the size of window.
           window size will be 1/winProportion of Img height.
           Default is 12.
        '''
        self._win_proportion = winProportion

class ExudateDataMaker(DataMakerBase):
    '''This class is used to make Exudates training data set
       It needs two images: OriginImg and LabelImg. 
       The results will be classified into 3 classes: exudates(2), disk(1), and background(0)
    '''
    def __init__(self):
        DataMakerBase.__init__(self)
        self._num_exu = 0
        self._num_disk = 0
        self._num_bg = 0
        
    def Apply(self, originPath, labelPath, outputDir, dataName):
        '''Create Dataset in 3 classes: [2:exudates, 1:disk, 0:background]
           Parameters: originPath is the Path of origin image.
                       labelPath is the Path of label image.
                       outputDir is the directory of results
                       dataName is the name of patches results
        '''
        originImg = cv2.imread(originPath)
        labelImg = cv2.imread(labelPath)
        window = [originImg.shape[0]/self._win_proportion, originImg.shape[0]/self._win_proportion, 3]
        
        
        Path_2 = outputDir +'/2_exudate/'
        if os.path.exists(Path_2) is False:
            os.system('mkdir '+Path_2)
        Path_1 = outputDir +'/1_disk/'
        if os.path.exists(Path_1) is False:
            os.system('mkdir '+Path_1)
        Path_0 = outputDir +'/0_background/'
        if os.path.exists(Path_0) is False:
            os.system('mkdir '+Path_0)
        
        iter_label = labelImg.flat
        
        for i in xrange(labelImg.size/3):
            value_b = iter_label.next()
            value_g = iter_label.next()
            value_r = iter_label.next()
            if value_b>=30 and value_b<120:
                outputPath = Path_0 + dataName +'_'+ str(self._num_bg)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_bg = self._num_bg+1
            elif value_b>=120 and value_b<220:
                outputPath = Path_1 + dataName +'_'+ str(self._num_disk)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_disk = self._num_disk+1
            elif value_b>=220:
                outputPath = Path_2 + dataName +'_'+ str(self._num_exu)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_exu = self._num_exu+1

class HamorDataMaker(DataMakerBase):
    '''This class is used to make Exudates training data set
       It needs two images: OriginImg and LabelImg. 
       The results will be classified into 3 classes: hamor(3), edge(2), vessel(1), and background(0)
    '''
    def __init__(self):
        DataMakerBase.__init__(self)
        self._num_bg = 0
        self._num_ves = 0
        self._num_edge = 0
        self._num_hamor = 0
        
    def Apply(self, originPath, labelPath, outputDir, dataName):
        '''Create Dataset in 3 classes: [2:exudates, 1:disk, 0:background]
           Parameters: originPath is the Path of origin image.
                       labelPath is the Path of label image.
                       outputDir is the directory of results
                       dataName is the name of patches results
        '''
        originImg = cv2.imread(originPath)
        labelImg = cv2.imread(labelPath)
        window = [originImg.shape[0]/self._win_proportion, originImg.shape[0]/self._win_proportion, 3]
        
        
        Path_0 = outputDir +'/0_background/'
        if os.path.exists(Path_0) is False:
            os.system('mkdir '+Path_0)
        Path_1 = outputDir +'/1_vessel/'
        if os.path.exists(Path_1) is False:
            os.system('mkdir '+Path_1)
        Path_2 = outputDir +'/2_edge/'
        if os.path.exists(Path_2) is False:
            os.system('mkdir '+Path_2)
        Path_3 = outputDir +'/3_hamor/'
        if os.path.exists(Path_3) is False:
            os.system('mkdir '+Path_3)
        
        Gray_num = 0
        Red_num = 0
        Blue_num = 0
        White_num = 0
        Total_num = 0
        iter_label = labelImg.flat
        for i in xrange(labelImg.size/3):
            value_b = iter_label.next()
            value_g = iter_label.next()
            value_r = iter_label.next()
            #Gray is background
            if value_b>=30 and value_b<150 and value_g>=30 and value_g<150 and value_r>=30 and value_r<150:
                Gray_num = Gray_num+1
                Total_num = Total_num+1
            #Red is vessel
            elif value_b<30 and value_g<30 and value_r>220:
                Red_num = Red_num+1
                Total_num = Total_num+1
            #Blue is edge
            elif value_b>=220 and value_g<30 and value_r<30:
                Blue_num = Blue_num+1
                Total_num = Total_num+1
            #White is Hamorrage
            elif value_b>=220 and value_g>=220 and value_r>=220:
                White_num = White_num+1
                Total_num = Total_num+1
        p_Gray = 1.0-float(Gray_num)/Total_num
        p_Red = 1.0-float(Red_num)/Total_num
        p_Blue = 1.0-float(Blue_num)/Total_num
        p_White = 1.0-float(White_num)/Total_num
        
        iter_label = labelImg.flat
        for i in xrange(labelImg.size/3):
            value_b = iter_label.next()
            value_g = iter_label.next()
            value_r = iter_label.next()
            #Gray is background
            if random.random()<p_Gray and value_b>=30 and value_b<150 and value_g>=30 and value_g<150 and value_r>=30 and value_r<150:
                outputPath = Path_0 + dataName +'_'+ str(self._num_bg)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_bg = self._num_bg+1
            #Red is vessel
            elif random.random()<p_Red and value_b<30 and value_g<30 and value_r>220:
                outputPath = Path_1 + dataName +'_'+ str(self._num_ves)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_ves = self._num_ves+1
            #Blue is edge
            elif random.random()<p_Blue and value_b>=220 and value_g<30 and value_r<30:
                outputPath = Path_2 + dataName +'_'+ str(self._num_edge)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_edge = self._num_edge+1
            #White is Hamorrage
            elif random.random()<p_White and value_b>=220 and value_g>=220 and value_r>=220:
                outputPath = Path_3 + dataName +'_'+ str(self._num_hamor)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_hamor = self._num_hamor+1

class HamorDataMaker2(DataMakerBase):
    '''This class is used to make Exudates training data set
       It needs two images: OriginImg and LabelImg. 
       The results will be classified into 3 classes: hamor(2), vessel(1), and background(0)
       Version Info: Merge Gray with blue
    '''
    def __init__(self):
        DataMakerBase.__init__(self)
        self._num_bg = 0
        self._num_ves = 0
        self._num_edge = 0
        self._num_hamor = 0
        
    def Apply(self, originPath, labelPath, outputDir, dataName):
        '''Create Dataset in 3 classes: [2:exudates, 1:disk, 0:background]
           Parameters: originPath is the Path of origin image.
                       labelPath is the Path of label image.
                       outputDir is the directory of results
                       dataName is the name of patches results
        '''
        originImg = cv2.imread(originPath)
        labelImg = cv2.imread(labelPath)
        window = [originImg.shape[0]/self._win_proportion, originImg.shape[0]/self._win_proportion, 3]
        
        
        Path_0 = outputDir +'/0_background/'
        if os.path.exists(Path_0) is False:
            os.system('mkdir '+Path_0)
        Path_1 = outputDir +'/1_vessel/'
        if os.path.exists(Path_1) is False:
            os.system('mkdir '+Path_1)
        Path_2 = outputDir +'/2_hamor/'
        if os.path.exists(Path_2) is False:
            os.system('mkdir '+Path_2)

        
        Gray_num = 0
        Red_num = 0
        White_num = 0
        Total_num = 0
        iter_label = labelImg.flat
        for i in xrange(labelImg.size/3):
            value_b = iter_label.next()
            value_g = iter_label.next()
            value_r = iter_label.next()
            #Gray is background
            if value_b>=30 and value_b<150 and value_g>=30 and value_g<150 and value_r>=30 and value_r<150:
                Gray_num = Gray_num+1
                Total_num = Total_num+1
            #Red is vessel
            elif value_b<30 and value_g<30 and value_r>220:
                Red_num = Red_num+1
                Total_num = Total_num+1
            #Blue is edge (Merge To Gray)
            elif value_b>=220 and value_g<30 and value_r<30:
                Gray_num = Gray_num+1
                Total_num = Total_num+1
            #White is Hamorrage
            elif value_b>=220 and value_g>=220 and value_r>=220:
                White_num = White_num+1
                Total_num = Total_num+1
        p_Gray = 1.0-float(Gray_num)/Total_num
        p_Red = 1.0-float(Red_num)/Total_num
        p_White = 1.0-float(White_num)/Total_num
        
        iter_label = labelImg.flat
        for i in xrange(labelImg.size/3):
            value_b = iter_label.next()
            value_g = iter_label.next()
            value_r = iter_label.next()
            #Gray is background
            if random.random()<p_Gray and value_b>=30 and value_b<150 and value_g>=30 and value_g<150 and value_r>=30 and value_r<150:
                outputPath = Path_0 + dataName +'_'+ str(self._num_bg)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_bg = self._num_bg+1
            #Red is vessel
            elif random.random()<p_Red and value_b<30 and value_g<30 and value_r>220:
                outputPath = Path_1 + dataName +'_'+ str(self._num_ves)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_ves = self._num_ves+1
            #Blue is edge (Merge To Gray)
            elif random.random()<p_Gray and value_b>=220 and value_g<30 and value_r<30:
                outputPath = Path_0 + dataName +'_'+ str(self._num_bg)+'.jpg'
                cv2.imwrite(outputPath, self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))
                self._num_bg = self._num_bg+1
            #White is Hamorrage
            elif random.random()<p_White and value_b>=220 and value_g>=220 and value_r>=220:
                outputPath = Path_2 + dataName +'_'+ str(self._num_hamor)+'.jpg'
                cv2.imwrite(outputPath, np.fliplr(np.flipud(self._GetPatch(originImg, iter_label.coords[0], iter_label.coords[1], window))))
                self._num_hamor = self._num_hamor+1               

#Test the method        
if __name__=='__main__': 
    for i in [1]:
        originPath = '/home/linhuangjing/Desktop/Retina_Data/Group-0/DR raw/DR-'+str(i)+'.jpg'
        labelPath = '/home/linhuangjing/Desktop/DR_result/DR_result'+str(i)+'.jpg'
        outputDir = '/home/linhuangjing/Desktop/Exudate_Data'
        dataName = 'DR_'+str(i)+'_t'
#     for i in xrange(1,5):
#         originPath = '/home/linhuangjing/Desktop/Retina_Data/Group-0/DR raw/DR-'+str(i)+'.jpg'
#         labelPath = '/home/linhuangjing/Desktop/Retina_Data/Group-0/Version2.0 80/DR_result/DR_result'+str(i)+'.jpg'
#         outputDir = '/home/linhuangjing/Desktop/Retina_Data/Debug/TestingData'
#         dataName = 'DR_'+str(i)+'_t'
        
        dataMaker = ExudateDataMaker()
        dataMaker.Apply(originPath, labelPath, outputDir, dataName)
        print 'Finish!'+str(i)
