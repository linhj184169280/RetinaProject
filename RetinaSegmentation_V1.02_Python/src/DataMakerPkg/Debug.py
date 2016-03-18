'''
Created on Mar 27, 2015

@author: linhuangjing
'''
import DataMaker
import time
import LabelDataToTxt

originPath = '/home/linhuangjing/Desktop/Retina_Data/Debug/DR-1.jpg'
labelPath = '/home/linhuangjing/Desktop/Retina_Data/Debug/DR_result1.jpg'
outputDir = '/home/linhuangjing/Desktop/Retina_Data/Debug/Dataset'
dataName = 'DR1'


time1 = time.time()
    
dataMaker = DataMaker.HamorDataMaker()
#dataMaker.Apply(originPath, labelPath, outputDir, dataName)
#LData2Txt = HamorLabelDataToTxt.LabelDataToTxt()
#LData2Txt.Apply(outputDir, outputDir+'/testdata.txt')
    
time2 = time.time()
alltime=time2-time1
print alltime


