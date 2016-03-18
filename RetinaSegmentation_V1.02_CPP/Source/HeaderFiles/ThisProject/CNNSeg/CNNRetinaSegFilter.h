/*
 * CNNRetinaSegFilter.h
 *
 *  Created on: Apr 21, 2015
 *      Author: linhuangjing
 */

#ifndef CNNRETINASEGFILTER_H_
#define CNNRETINASEGFILTER_H_

#include <iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;

#include "PreprocessingModule.h"
#include "Classifier.h"

bool MergeImages(Mat imgL, Mat imgR, Mat& result)
{
	if(imgL.cols!=imgR.cols)
		return false;
	cv::Mat t_result(imgL.rows,imgL.cols+imgR.cols+5,CV_32FC3);

	imgL.convertTo(imgL,CV_32FC3);
	imgR.convertTo(imgR,CV_32FC3);
	MatIterator_<Vec3f> it_imgL, end_imgL;
	MatIterator_<Vec3f> it_imgR, end_imgR;

	for(it_imgL=imgL.begin<Vec3f>(),end_imgL=imgL.end<Vec3f>();it_imgL!=end_imgL;++it_imgL)
	{
		t_result.at<Vec3f>(it_imgL.pos().y,it_imgL.pos().x)[0]=(*it_imgL)[0];
		t_result.at<Vec3f>(it_imgL.pos().y,it_imgL.pos().x)[1]=(*it_imgL)[1];
		t_result.at<Vec3f>(it_imgL.pos().y,it_imgL.pos().x)[2]=(*it_imgL)[2];
	}

	for(it_imgR=imgR.begin<Vec3f>(),end_imgR=imgR.end<Vec3f>();it_imgR!=end_imgR;++it_imgR)
	{
		t_result.at<Vec3f>(it_imgR.pos().y,it_imgR.pos().x+imgL.cols+5)[0]=(*it_imgR)[0];
		t_result.at<Vec3f>(it_imgR.pos().y,it_imgR.pos().x+imgL.cols+5)[1]=(*it_imgR)[1];
		t_result.at<Vec3f>(it_imgR.pos().y,it_imgR.pos().x+imgL.cols+5)[2]=(*it_imgR)[2];
	}
	result = t_result;
	return true;
}

void ExudateSegment(Mat originImg, Mat& resultImg)
{
	Mat featureImg,presegImg;

	cout<<"Preprocessing..."<<endl;

	PreExudate(originImg, presegImg);
	//originImg to be classification feature.
	featureImg = originImg;

	presegImg.convertTo(presegImg,CV_8UC1);
	cv::medianBlur(presegImg,presegImg,3);

	MatIterator_<uchar> it, end;
	for(it = presegImg.begin<uchar>(),end = presegImg.end<uchar>(); it!=end; it++)
		if ((*it)>100)
			(*it) = 80;
		else
			(*it) =0;

	Classifier classifier("../Configurations/retina_deploy.prototxt", "../Configurations/Exudate_iter_74000.caffemodel", "../Configurations/Exudate_image_mean.binaryproto");
	Classifier::Prediction prediction;

	cv::Mat t_result(originImg.rows,originImg.cols,CV_32FC3);
	featureImg.convertTo(featureImg,CV_32FC3);
	originImg.convertTo(originImg,CV_32FC3);

	MatIterator_<uchar> it_pre, end_pre;
	MatIterator_<Vec3f> it_fea, end_fea;
	MatIterator_<Vec3f> it_ori, end_ori;
	MatIterator_<Vec3f> it_res, end_res;

	int windowSize = originImg.rows/12;
	windowSize = windowSize/2*2;


	for( it_pre = presegImg.begin<uchar>(), end_pre = presegImg.end<uchar>(), it_fea = featureImg.begin<Vec3f>(), end_fea = featureImg.end<Vec3f>(),
			it_res = t_result.begin<Vec3f>(), end_res = t_result.end<Vec3f>(),it_ori = featureImg.begin<Vec3f>(), end_ori = featureImg.end<Vec3f>();
			it_pre != end_pre; ++it_pre,++it_fea,++it_res,++it_ori)
	{
		(*it_res)[0]=(*it_ori)[0];
		(*it_res)[1]=(*it_ori)[1];
		(*it_res)[2]=(*it_ori)[2];
		if( (*it_pre)>40)
		{
			cv::Mat patch;
			cv::getRectSubPix(featureImg,Size(windowSize,windowSize),it_fea.pos(),patch,CV_32FC3);
			prediction = classifier.predict(patch);

			//Set to be white
			if(prediction.label==2&&prediction.propability>0.75)
			{
				(*it_res)[0]=(*it_res)[2]=(*it_res)[1]=255;
			}
			//set to green
//			if(prediction.label==1&&prediction.propability>0.75)
//			{
//				(*it_res)[1]=(*it_res)[3]=255;
//				(*it_res)[0]=(*it_res)[2]=0;
//			}
//			if(prediction.label==0&&prediction.propability>0.75)
//			{
//				(*it_res)[0]=(*it_res)[3]=255;
//				(*it_res)[1]=(*it_res)[2]=0;
//			}

		}

	}

//	cv::medianBlur(t_result,t_result,5);

	MergeImages(originImg, t_result,t_result);

	resultImg = t_result;

}


void HamorSegment(Mat originImg, Mat& resultImg, Mat& enhancedImg)
{
	Mat featureImg, presegImg;
	PreHamor2(originImg, featureImg, presegImg, enhancedImg);

	Mat JPImg;
	Mat originImg_b;
	cv::blur(originImg,originImg_b,Size(3,3));
	JPPreHamor(originImg_b, JPImg);

	CV_Assert(presegImg.depth() != sizeof(uchar));
	CV_Assert(JPImg.depth() != sizeof(uchar));

	imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/MyPre_102.jpg", presegImg);

	MatIterator_<uchar> it1, end1;
	MatIterator_<uchar> it2, end2;
	for( it1 = presegImg.begin<uchar>(), end1 = presegImg.end<uchar>(), it2 = JPImg.begin<uchar>(), end2 = JPImg.end<uchar>(); it1 != end1; ++it1,++it2)
	{
		if( (*it1)>(*it2) )
			(*it1) = (*it2);
	}

	imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/JPPre_102.jpg", JPImg);
	imwrite("/home/linhuangjing/Desktop/Retina_Data/Group-1/results/Pre_102.jpg", presegImg);

	Classifier classifier("../Configurations/retina_deploy.prototxt", "../Configurations/Hamor_iter_126000.caffemodel", "../Configurations/Hamor2_image_mean.binaryproto");
	Classifier::Prediction prediction;

	cv::Mat t_result(originImg.rows,originImg.cols,CV_32FC3);
	featureImg.convertTo(featureImg,CV_32FC3);
	originImg.convertTo(originImg,CV_32FC3);

	MatIterator_<uchar> it_pre, end_pre;
	MatIterator_<Vec3f> it_fea, end_fea;
	MatIterator_<Vec3f> it_ori, end_ori;
	MatIterator_<Vec3f> it_res, end_res;

	int windowSize = originImg.rows/12;
	windowSize = windowSize/2*2;


	for( it_pre = presegImg.begin<uchar>(), end_pre = presegImg.end<uchar>(), it_fea = featureImg.begin<Vec3f>(), end_fea = featureImg.end<Vec3f>(),
			it_res = t_result.begin<Vec3f>(), end_res = t_result.end<Vec3f>(),it_ori = originImg.begin<Vec3f>(), end_ori = originImg.end<Vec3f>();
			it_pre != end_pre; ++it_pre,++it_fea,++it_res,++it_ori)
	{
		(*it_res)[0]=(*it_ori)[0];
		(*it_res)[1]=(*it_ori)[1];
		(*it_res)[2]=(*it_ori)[2];
		if( (*it_pre)>40)
		{
			cv::Mat patch;
			cv::getRectSubPix(featureImg,Size(windowSize,windowSize),it_fea.pos(),patch,CV_32FC3);
			prediction = classifier.predict(patch);

			if(prediction.label==2)
			{
				(*it_res)[0]=(*it_res)[1]=(*it_res)[2]=255;
			}

		}

	}

	cv::medianBlur(t_result,t_result,5);


	MergeImages(enhancedImg, t_result,t_result);
	resultImg = t_result;

}

void RetinaAllSegment(Mat originImg, Mat& resultImg, Mat& enhancedImg)
{
	Mat ExuResult, HamorResult;
	ExudateSegment(originImg, ExuResult);
	HamorSegment(originImg, HamorResult, enhancedImg);

	cv::Mat t_result(originImg.rows,originImg.cols,CV_32FC4);
	MatIterator_<Vec4f> it_res, end_res;
	MatIterator_<Vec4f> it_exu, end_exu;
	MatIterator_<Vec4f> it_ham, end_ham;
	for(it_res = t_result.begin<Vec4f>(),end_res = t_result.end<Vec4f>(),it_exu = ExuResult.begin<Vec4f>(),end_exu = ExuResult.end<Vec4f>(),
			it_ham = HamorResult.begin<Vec4f>(),end_ham = HamorResult.end<Vec4f>(); it_res!=end_res; ++it_res,++it_exu,++it_ham)
	{
		(*it_res)[0]=(*it_res)[1]=(*it_res)[2]=(*it_res)[3]=0;
		if((*it_exu)[0]>80)
			(*it_res)[0]=(*it_res)[1]=(*it_res)[2]=(*it_res)[3]=255;
		else if((*it_ham)[0]>80)
			(*it_res)[1]=(*it_res)[3]=255;
	}
	resultImg = t_result;
}


#endif /* CNNRETINASEGFILTER_H_ */
