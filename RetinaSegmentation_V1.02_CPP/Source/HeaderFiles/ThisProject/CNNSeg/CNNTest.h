/*
 * CNNTest.h
 *
 *  Created on: Apr 22, 2015
 *      Author: linhuangjing
 */

#ifndef CNNTEST_H_
#define CNNTEST_H_

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


void TestCNN()
{
	Caffe::set_mode(Caffe::GPU);

	Net<float>* caffe_test_net = new Net<float>("../Configurations/retina_deploy.prototxt",caffe::TEST);
	caffe_test_net->CopyTrainedLayersFrom("../Configurations/Hamor_iter_126000.caffemodel");

	ifstream ifs ("../Configurations/Hamor2_image_mean.binaryproto",ios::binary);


//	if (ifs)
//	{
		ifstream* ifsp = &ifs;
		istream* istp = dynamic_cast<istream*>(ifsp);

		BlobProto tblob_proto;
		tblob_proto.ParseFromIstream(istp);
		cout<<"dims="<<tblob_proto.data(0)<<"  "<<tblob_proto.data(632)<<endl;

		//手动转opencv 的Mat
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

		cout<<"Mean="<<mean_mat.at<Vec3f>(0,2)[0]<<"  "<<mean_mat.at<Vec3f>(0,2)[2]<<endl;

		imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/AA_101.jpg", mean_mat);


//		cout<<"Mat="<<mat.channels()<<"  "<<mat.at<Vec3f>(0,0)<<endl;

//
//		Datum tdatum;
//		Datum* tdatum_p;
//		tdatum_p = &tdatum;
//		tdatum.set_data(tblob_proto.DebugString());
//		tdatum.set_channels(tblob_proto.channels());
//		tdatum.set_height(tblob_proto.height());
//		tdatum.set_width(tblob_proto.width());

//	}

//	Datum datum;
//	if(!ReadImageToDatum("/home/linhuangjing/Desktop/ExperTmp/v1.jpg",3,115,115,&datum))
//	{
//		cout<<"Read Error!"<<endl;
//		return ;
//	}
	Mat img = imread("/home/linhuangjing/Desktop/ExperTmp/h2.jpg");
//	Mat img = imread("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/AA_101.jpg");

	img.convertTo(img,CV_32FC3);

	cv::resize(img,img,Size(115,115));

//	img = img-mean_mat;

	cout<<"Img="<<img.at<Vec3f>(20,25)[0]<<"  "<<img.at<Vec3f>(43,52)[2]<<endl;

	imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/BB_101.jpg", img);



//	img.convertTo(img,CV_8UC3);

//	caffe::CVMatToDatum(img,&datum);

	//Get the blob

	Blob<float>* blob = new Blob<float>( 1, img.channels(), img.rows, img.cols );


	//Get the blobproto
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
	for(int c=0; c<img.channels(); c++)
		for(int h=0; h<img.rows; h++)
			for(int w=0; w<img.cols; w++)
			{
				blob_proto.set_data(i, blob_proto.data(i)+img.at<Vec3f>(h,w)[c]);
				i++;
			}

	for(int i=0;i<data_size;i++)
	{
		blob_proto.set_data(i, blob_proto.data(i)-tblob_proto.data(i));
	}



//	MatIterator_<float> it1, end1;
//	int i=0;
//	for(i=0, it1=img.begin<float>(), end1 = img.end<float>(); i<size_in_datum, it1!=end1 ; i++, ++it1)
//	{
//		blob_proto.set_data(i, blob_proto.data(i)+(*it1));
//	}
	cout<<"i="<<i<<"  "<<size_in_datum<<endl;
	//set data into blob
	blob->FromProto(blob_proto);


	//fill the vector
	vector<Blob<float>*> bottom;
	bottom.push_back(blob);

	float type = 0.0;
	const vector<Blob<float>*>& result = caffe_test_net->Forward(bottom, &type);

	float max = 0;
	float max_i = 0;
	float max2 = 0;
	float max_i2 = 0;

	for(int i=0;i<3;i++)
	{
		float value = result[0]->cpu_data()[i];
		if(max<value){
			max = value;
			max_i = i;
		}
	}
	cout<<"i: "<<max_i<<"   probablity: "<<max<<endl;


}





#endif /* CNNTEST_H_ */
