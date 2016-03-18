'''
Created on Apr 24, 2015

@author: linhuangjing
'''
import numpy as np
import cv2
import sys
from utils import GetPixel
from utils import GetPatch

from multiprocessing import Pool
from multiprocessing.dummy import Pool as ThreadPool
import pathos.pp_map as mp

import time
from numbapro import vectorize
from functools import partial

import copy_reg
import types

sys.path.insert(0, "/home/linhuangjing/caffe-master/python")
import caffe
caffe.set_mode_gpu()

blob = caffe.proto.caffe_pb2.BlobProto()
data = open("../../Configurations/Exudate_image_mean.binaryproto",'rb').read()
blob.ParseFromString(data)
arr = np.array( caffe.io.blobproto_to_array(blob) )
_mean_Img = arr[0]
        #create the CNN network
_net = caffe.Classifier("../../Configurations/retina_deploy.prototxt", "../../Configurations/Exudate_iter_74000.caffemodel", mean=_mean_Img, channel_swap=(0,1,2), input_scale=1.0, raw_scale=1.0)
        
patch = cv2.imread("/home/linhuangjing/Desktop/ExperTmp/ds1.jpg");   
patch=patch.astype(float)

batch=[]
batch.append(patch)

preds = _net.predict(batch, False)

print preds

print "Finish"






