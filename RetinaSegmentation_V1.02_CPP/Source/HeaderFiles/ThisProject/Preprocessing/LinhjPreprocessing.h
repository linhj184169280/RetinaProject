#include <iostream>
#include<opencv.hpp>

using namespace std;
using namespace cv;

void PreExudate(Mat src, Mat& presegImg)
{
	src.convertTo(src,CV_32FC3);

	vector<Mat> channels;
	Mat b,g,r;
	split(src,channels);
	b = channels.at(0);
	g = channels.at(1);
	r = channels.at(2);

	cv::Mat InImage(g.rows,g.cols,CV_32FC1);
	cv::addWeighted(b,0.333333,r,0.333333,0.0,InImage);
	cv::addWeighted(g,0.333333,InImage,1.0,0.0,InImage);

	int wsize = g.rows/30;
	if (wsize%2==0)
		wsize=wsize+1;

	InImage.convertTo(InImage,CV_8UC1);

	cv::Mat bgEst(InImage.rows,InImage.cols,CV_8UC1);
	cv::medianBlur(InImage,bgEst,wsize);

	cv::Mat bgMask1(InImage.rows,InImage.cols,CV_8UC1);
	cv::Mat bgMask2(InImage.rows,InImage.cols,CV_8UC1);


	MatIterator_<uchar> it1, end1;
	MatIterator_<uchar> it2, end2;
	MatIterator_<uchar> it_In, end_In;
	MatIterator_<uchar> it_Est, end_Est;

	for(it1 = bgMask1.begin<uchar>(),end1 = bgMask1.end<uchar>(),it2 = bgMask2.begin<uchar>(),end2 = bgMask2.end<uchar>()
			,it_In = InImage.begin<uchar>(),end_In = InImage.end<uchar>(),it_Est = bgEst.begin<uchar>(),end_Est = bgEst.end<uchar>();
			it1!=end1;++it1,++it2,++it_In,++it_Est)
	{
		if((*it_In)>(*it_Est))
		{
			(*it1) = (*it_In);
			(*it2) = (*it_Est);
		}
		else
		{
			(*it1) = (*it_Est);
			(*it2) = (*it_In);
		}
	}
	cv::Mat im1(InImage.rows,InImage.cols,CV_32FC1);
	cv::Mat im2(InImage.rows,InImage.cols,CV_32FC1);
	cv::Mat dst(InImage.rows,InImage.cols,CV_32FC1);


	bgEst.convertTo(bgEst,CV_32FC1);
	bgMask1.convertTo(bgMask1,CV_32FC1);
	bgMask2.convertTo(bgMask2,CV_32FC1);

	im1 = bgMask1-bgMask2;
	im2 = g-bgEst; //Type May different

	MatIterator_<float> it_im1, end_im1;
	MatIterator_<float> it_im2, end_im2;
	MatIterator_<float> it_dst, end_dst;

	for(it_im1 = im1.begin<float>(),end_im1 = im1.end<float>(),it_im2 = im2.begin<float>(),end_im2 = im2.end<float>(),
			it_dst = dst.begin<float>(),end_dst = dst.end<float>(); it_im1!=end_im1;++it_im1,++it_im2,++it_dst)
	{
		if((*it_im1)<8)
			(*it_im1) = 0;
		if((*it_im2)<6)
			(*it_im2) = 0;
		if((*it_im1)<(*it_im2))
			(*it_dst) = (*it_im1);
		else
			(*it_dst) = (*it_im2);
		if ((*it_dst)>5)
			(*it_dst) = 255;
		else
			(*it_dst) = 0;
	}

	presegImg = dst;
}


/* Hamorrhage Preprocessing function
 * "Mat src" is input image.
 * "Mat& featureImage" is feature image which is used to generate patches.
 * "Mat& PresegImg" is binarized tmpImg used to determine pixels which should be classified.
 * "Mat& tmpImg" is result processed by morphology.
 */
void PreHamor2(Mat src, Mat& featureImg, Mat& presegImg, Mat& enhancedImg)
{
//	featureImg=PresegImg=tmpImg=src;
	vector<Mat> channels;
	Mat b,g,r;

	split(src,channels);
	b = channels.at(0);
	g = channels.at(1);
	r = channels.at(2);

	int morph_size = 10;
	Mat kernel = getStructuringElement(cv::MORPH_CROSS,Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ));

	Mat blackhat;
	morphologyEx(g, blackhat, cv::MORPH_BLACKHAT, kernel);

//	addWeighted(g, 1.0, blackhat, -5.0, 0.0, enhancedImg);
	enhancedImg = g-5*blackhat;
	presegImg = enhancedImg.clone();

	CV_Assert(g.depth() != sizeof(uchar));

	MatIterator_<uchar> it1, end1;
	MatIterator_<Vec3b> it2, end2;
	for( it1 = presegImg.begin<uchar>(), end1 = presegImg.end<uchar>(), it2 = src.begin<Vec3b>(), end2 = src.end<Vec3b>(); it1 != end1; ++it1,++it2)
	{
		if ( (*it1)<30 && ((*it2)[0]>10 || (*it2)[1]>10 || (*it2)[2]>10) )
			(*it1) = 80;
		else
			(*it1) = 0;
	}

	channels[0] = enhancedImg;
	channels[1] = g;
	channels[2] = enhancedImg;
	merge(channels, featureImg);

	channels[0] = enhancedImg;
	channels[1] = enhancedImg;
	channels[2] = enhancedImg;
	merge(channels, enhancedImg);

}
