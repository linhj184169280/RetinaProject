/*
 * Classifier.h
 *
 *  Created on: Apr 24, 2015
 *      Author: linhuangjing
 */

#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <cuda_runtime.h>

#include <cstring>
#include <cstdlib>
#include <vector>

#include <string>
#include <iostream>
#include <stdio.h>
#include "caffe/caffe.hpp"
#include "caffe/util/io.hpp"
#include "caffe/blob.hpp"

#include<opencv.hpp>

#include <fstream>
#include "stdio.h"


using namespace caffe;
using namespace std;
using namespace cv;



class Classifier
{
public:
	struct Prediction
	{
		int label;
		float propability;
	};

	Classifier(string model_file, string pretrained_file, string mean_file, int input_height=115, int input_weight=115,int input_channel=3)
	{
		Caffe::set_mode(Caffe::GPU);
		//Load Network
		_net = new Net<float>(model_file,caffe::TEST);
		_net->CopyTrainedLayersFrom(pretrained_file);

		_input_height = input_height;
		_input_weight = input_weight;
		_input_channel = input_channel;

		//Load Mean Image
		ifstream ifs (mean_file,ios::binary);
		if (ifs)
		{
			ifstream* ifsp = &ifs;
			istream* istp = dynamic_cast<istream*>(ifsp);

			BlobProto tblob_proto;
			tblob_proto.ParseFromIstream(istp);

			//Transfer "MeanImage" from BlobProto into cv::Mat
			cv::Mat mean_mat(tblob_proto.height(),tblob_proto.width(),CV_32FC3);

			int step0 = 0; int step1 = tblob_proto.height()*tblob_proto.width(); int step2 = step1*2;
			int Size_img = tblob_proto.height()*tblob_proto.width();
			for (int i=0; i<tblob_proto.height(); i++)
			{
				for(int j=0;j<tblob_proto.width(); j++)
				{
					mean_mat.ptr<float>(i,j)[0] = tblob_proto.data(i*tblob_proto.width()+j);
					mean_mat.ptr<float>(i,j)[1] = tblob_proto.data(step1+i*tblob_proto.width()+j);
					mean_mat.ptr<float>(i,j)[2] = tblob_proto.data(step2+i*tblob_proto.width()+j);
				}
			}
			_mean_mat=mean_mat;
		}
	}
	Prediction predict(cv::Mat img)
	{
		img.convertTo(img,CV_32FC3);
		cv::resize(img,img,Size(_input_weight,_input_height));



		img = img-_mean_mat;


		BlobProto blob_proto;
		blob_proto.set_num(1);
		blob_proto.set_channels(img.channels());
		blob_proto.set_height(img.rows);
		blob_proto.set_width(img.cols);
		const int data_size = img.channels() * img.rows * img.cols;
		int size_in_datum = data_size;

		for (int i = 0; i<size_in_datum; i++)
		{
			blob_proto.add_data(0.);
		}
		int i=0;

		//Transfer img to blobproto type.
		for(int c=0; c<img.channels(); c++)
			for(int h=0; h<img.rows; h++)
				for(int w=0; w<img.cols; w++)
				{
					blob_proto.set_data(i, blob_proto.data(i)+img.at<Vec3f>(h,w)[c]);
					i++;
				}

		Blob<float>* blob = new Blob<float>( 1, img.channels(), img.rows, img.cols );
		blob->FromProto(blob_proto);

		//fill the vector
		vector<Blob<float>*> bottom;
		bottom.push_back(blob);

		float type = 0.0;
		const vector<Blob<float>*>& pred = _net->Forward(bottom, &type);

		vector<Blob<float>*>::iterator it;
		for(it=bottom.begin();it!=bottom.end();it++)
		{
			delete (*it);
		}

		float max = 0;
		float max_i = 0;
		float max2 = 0;
		float max_i2 = 0;

		for(int i=0;i<3;i++)
		{
			float value = pred[0]->cpu_data()[i];
			if(max<value){
				max = value;
				max_i = i;
			}
		}
//		cout<<"i: "<<max_i<<"   probablity: "<<max<<endl;

		Prediction prediction;
		prediction.label=max_i;
		prediction.propability=max;

		return prediction;
	}

private:
	Net<float>* _net;
	cv::Mat _mean_mat;
	int _input_channel;
	int _input_height;
	int _input_weight;


};




#endif /* CLASSIFIER_H_ */
