#include <iostream>
using namespace std;

#include "omp.h"
#include<string>
#include <sstream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<queue>
#include<cstdlib>
#include<cstdio>
#include<fstream>

#include<opencv.hpp>

#include "PreprocessingModule.h"
#include "CNNRetinaSegFilter.h"
#include "CNNTest.h"

#include "Classifier.h"
#include <time.h>

using namespace cv;

int main(int argc, char **argv)
{

    string sourcePath = "/home/linhuangjing/Desktop/Retina_Data/Group-0/DR raw/DR-2.jpg";
    string ExuResultPath = "/home/linhuangjing/Desktop/Retina_Data/Group-0/results/DR_Exu_2.jpg";
    string HamResultPath = "/home/linhuangjing/Desktop/Retina_Data/Group-0/results/DR_Ham_2.jpg";

//    string sourcePath = argv[1];
//    string ExuResultPath = argv[2];
//    string HamResultPath = argv[3];

    Mat img = imread(sourcePath);
    int rows=img.rows,cols=img.cols;

    time_t stime , etime;
    stime = time(NULL); /* get start time */

    Mat ExuResultImg, HamResultImg, enhancedImg;

    ExudateSegment(img,ExuResultImg);
    HamorSegment(img, HamResultImg, enhancedImg);


	imwrite(ExuResultPath, ExuResultImg);
	imwrite(HamResultPath, HamResultImg);


    etime = time(NULL); /* get end time */

	cout<<"Time = "<<(etime - stime)<<endl;

//	TestCNN();

//	Classifier classifier("../Configurations/retina_deploy.prototxt", "../Configurations/Exudate_iter_74000.caffemodel", "../Configurations/Exudate_image_mean.binaryproto");
//
////	string sourceimg="/home/linhuangjing/Desktop/Retina_Data/Group-1/DR/DR_101.jpg";
//	Mat img = imread("/home/linhuangjing/Desktop/ExperTmp/ex1.jpg");
//
//	Classifier::Prediction result;
//	result = classifier.predict(img);
//
//	cout<<"i="<<result.label<<"  probability="<<result.propability<<endl;


	cout<<"Complete"<<endl;

	return 0;
}
