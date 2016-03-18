#include "omp.h"
#include<opencv.hpp>

#include <iostream>
#include<string>
#include <sstream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<queue>
#include<cstdlib>
#include<cstdio>
#include<fstream>

using namespace std;
using namespace cv;
struct point
{
	int x, y;
};
string inttostring(int a)
{
	stringstream ss;
	string s;
	ss << a;
	ss >> s;
	return s;
}
Mat letresize(Mat img1)//图像大小变为统一的
{
	double b = (double)img1.rows / (double) 565.0;
	int a = img1.cols / b;
	resize(img1, img1, Size(a, 565));
	return img1;
}

void avglight(Mat &img,int width,int under=25)
{
	medianBlur(img, img, 3);
	Mat clone = img.clone();
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			int tot = 0, totnum = 0;
			int tempyy = y - width, tempxx = x - width;
			if (tempyy < 0) tempyy = 0;
			if (tempxx < 0) tempxx = 0;
			if (img.at<uchar>(y, x) < 15) continue;
			for (int yy = tempyy; yy < img.rows&&yy < tempyy + 2 * width; yy++)
			{
				for (int xx = tempxx; xx < img.cols&&xx < tempxx + 2 * width; xx++)
				{
					if (clone.at<uchar>(yy, xx) <under) continue;
					tot += clone.at<uchar>(yy, xx);
					totnum++;
				}
			}
			if (totnum)
			{
				img.at<uchar>(y, x) = img.at<uchar>(y, x) + 120 - tot / totnum;
			}
		}
	}
}


void allavglight(Mat &img, int width)
{
	medianBlur(img, img, 3);
	Mat clone = img.clone();
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			int tot = 0, totnum = 0;
			int tempyy = y - width, tempxx = x - width;
			if (tempyy < 0) tempyy = 0;
			if (tempxx < 0) tempxx = 0;
			if (img.at<Vec3b>(y, x)[0] < 25 && img.at<Vec3b>(y, x)[1] < 25 && img.at<Vec3b>(y, x)[2] < 25) continue;
			for (int yy = tempyy; yy < img.rows&&yy < tempyy + 2 * width; yy++)
			{
				for (int xx = tempxx; xx < img.cols&&xx < tempxx + 2 * width; xx++)
				{
					if (clone.at<Vec3b>(yy, xx)[0] <25 && clone.at<Vec3b>(yy, xx)[1] <25 && clone.at<Vec3b>(yy, xx)[2] <25) continue;
					tot += clone.at<Vec3b>(yy, xx)[0] + clone.at<Vec3b>(yy, xx)[1] + clone.at<Vec3b>(yy, xx)[2];
					totnum++;
				}
			}
			if (totnum)
			{
				img.at<Vec3b>(y, x)[0] = img.at<Vec3b>(y, x)[0] + 120 - tot / (totnum * 3);
				img.at<Vec3b>(y, x)[1] = img.at<Vec3b>(y, x)[1] + 120 - tot / (totnum * 3);
				img.at<Vec3b>(y, x)[2] = img.at<Vec3b>(y, x)[2] + 120 - tot / (totnum * 3);
			}
		}
	}
}

void killcenter(Mat source, Mat old)
{

	Mat green = source.clone();
	medianBlur(source, green, 21);

	vector<int> tots;
	int totsum = 0;
	for (int y = 0; y < green.rows; y++)
	{
		int sum = 0;
		for (int x = 0; x < green.cols; x++)
		{
			if (source.at<uchar>(y, x) < 140)
				sum += 0; //120;

			else sum += source.at<uchar>(y, x);

		}
		tots.push_back(sum);
		totsum += sum;
	}

	int l = 0, minisum = totsum - tots[0] - tots[0], up = tots[0];
	vector<int> why;
	for (int i = 1; i < tots.size(); i++)//找到纵坐标
	{
		up += tots[i];
		if (abs(totsum - up - up) < minisum)
		{
			minisum = abs(totsum - up - up);
			l = i;
		}
	}

	int m = 0;
	double min = 5000 * 255;

	vector<int> v;
	for (int x = 0; x < green.cols - 0; x++)
	{
		int avg = 0, num = 0;
		for (int y = l - 80; y <= l + 80; y++)
		{
			for (int xx = x - 80; xx <= x + 80; xx++)
			{
				if (y<0||y>=green.rows||xx<0 || xx >= green.cols || green.at<uchar>(y, xx) < 70) continue;//<70是因为外面的圆与黑色的交界处有70以内的点
				avg += green.at<uchar>(y, xx);
				num++;
			}
		}
		if (x == 158 || x == 286) cout << num << endl;
		if (num)avg = avg / num;
		else avg = 0;

		int sum = 0;
		for (int y = l - 20; y <= l + 20; y++)
		{
			for (int xx = x - 80; xx <= x + 80; xx++)
			{
				if (y<0 || y >= green.rows || xx<0 || xx >= green.cols || y<0 || y >= green.rows || green.at<uchar>(y, xx) < 70) continue;
				sum = sum + (green.at<uchar>(y, xx) - avg)*(green.at<uchar>(y, xx) - avg);
			}
		}
		/*if (sum > max)
		{
		m = x;
		max = sum;
		}*/
		v.push_back(sum);
	}

	vector<int>g;
	for (int x = 0; x < green.cols; x++)
	{
		vector<int> sum;
		for (int yy = l - 100; yy <= l + 100; yy++)
		{
			for (int xx = x - 10; xx < x + 10; xx++)
			{
				if (yy<0 || yy >= green.rows||xx<0 || xx >= green.cols || green.at<uchar>(yy, xx) < 70) continue;
				sum.push_back(green.at<uchar>(yy, xx));
			}
		}
		int t = 500;
		if (sum.size() < 4000)
		{
			g.push_back(5000 * 255);
		}
		else
		{
			sort(sum.begin(), sum.end());
			int temp = 0;
			for (int i = 0; i < 5; i++) temp += sum[i];
			/*if (x == 127)
			{
			for (int i = 0; i < 20; i++) cout << sum[i]<<" ";
			}*/
			g.push_back(temp);
		}
	}
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] == 0) continue;
		if ((double)g[i] / (double)v[i] < min)
		{
			m = i;
			min = (double)g[i] / (double)v[i];
		}
	}
	/*cout << v[127] << " " << g[127] << endl;
	cout << v[286] << " " << g[286] << endl;
	*/




	for (int y = l - 85; y < l + 85; y++)
	{
		for (int x = m - 85; x < m + 85; x++)
		{
			try
			{
				old.at<uchar>(y, x) = 0;
			}
			catch (Exception ex)
			{
				int a;
				a++;
			}
		}
	}

}




void findcentermax(Mat green)
{
	vector<int> tots;
	int totsum = 0;
	for (int y = 0; y < green.rows; y++)
	{
		int sum = 0;
		for (int x = 0; x < green.cols; x++)
		{
			try
			{
				sum += green.at<uchar>(y, x);
			}
			catch (exception ex)
			{
				int a;
				a++;
			}

		}
		tots.push_back(sum);
		totsum += sum;
	}

	int l = 0, minisum = totsum - tots[0] - tots[0], up = tots[0];
	vector<int> why;
	for (int i = 1; i < tots.size(); i++)//找到纵坐标
	{
		up += tots[i];
		if (abs(totsum - up - up) < minisum)
		{
			minisum = abs(totsum - up - up);
			l = i;
		}
	}

	int m = 0;
	double max = 0;

	vector<int> v;
	for (int x = 0; x < green.cols - 0; x++)
	{
		int sum = 0;
		for (int y = l - 50; y <= l + 50; y++)
		{
			for (int xx = x - 50; xx <= x + 50; xx++)
			{
				if (xx<0 || xx >= green.cols) continue;
				sum += green.at<uchar>(y, xx);
			}
		}
		if (sum > max)
		{
			m = x;
			max = sum;
		}

	}





	for (int y = l - 75; y < l + 75; y++)
	{
		for (int x = m - 75; x < m + 75; x++)
		{
			try
			{
				green.at<uchar>(y, x) = 255;
			}
			catch (Exception ex)
			{
				int a;
				a++;
			}
		}

	}
}


int  lhImageCmp(const IplImage* img1, const IplImage* img2)
{
	assert(img1->width == img2->width && img1->height == img2->height && img1->imageSize == img2->imageSize);
	return memcmp(img1->imageData, img2->imageData, img1->imageSize);
}

void markans(Mat old, Mat morans, int mark=255)
{
	for (int y = 0; y < old.rows; y++)
	{
		for (int x = 0; x < old.cols; x++)
		{
			if (morans.at<uchar>(y, x) > 0)
			{
				if (old.channels() == 3)
				{
					old.at<Vec3b>(y, x)[0] = 0;
					old.at<Vec3b>(y, x)[1] = mark;
					old.at<Vec3b>(y, x)[2] = 0;
				}
				else old.at<uchar>(y, x) = mark;

			}

		}
	}
}

void markansbyOrignialColor(Mat old, Mat morans)//用原来的颜色标记图案
{
	int pixel;
	for (int y = 0; y < old.rows; y++)
	{
		for (int x = 0; x < old.cols; x++)
		{
			pixel = morans.at<uchar>(y, x);
			if (pixel > 0)
			{
				old.at<Vec3b>(y, x)[0] = 0;
				old.at<Vec3b>(y, x)[1] = pixel;
				old.at<Vec3b>(y, x)[2] = 0;
			}

		}
	}
}

Mat matsubtract(Mat a, Mat b)//图像相减，小于0的部分为0
{
	Mat ans = a.clone();
	const int channels = a.channels();
	switch (channels)
	{
	case 1:
	{
		MatIterator_<uchar> it1,it2,it3, end1,end2,end3;
		for (it1 = a.begin<uchar>(), it2 = b.begin<uchar>(), it3 = ans.begin<uchar>(), end1 = a.end<uchar>(), end2 = b.end<uchar>(), end3 = ans.end<uchar>(); it1 != end1;++it1, ++it2,++it3)
		{
			int temp = *it1 - *it2;
			if (temp > 255)
			{
				*it3 = 255;
				continue;
			}
			if (temp > 0)
			{
				*it3 = temp;
			}
			else *it3 = 0;
		}
		break;
	}
	}
	return ans;
}
void aboveplus(Mat a, int above, int plus)//如果a》above则加上plus否则为0
{
	MatIterator_<uchar> it, end;
	for (it = a.begin<uchar>(), end = a.end<uchar>(); it != end; ++it)
	{
		*it = *it > above ? *it+plus : 0;
	}
}
void killout(Mat oldgreen, Mat green)
{
	Mat element2 = getStructuringElement(MORPH_RECT, Size(10,10));//核的大小
	morphologyEx(oldgreen, oldgreen, CV_MOP_ERODE, element2);
	MatIterator_<uchar> it1,it2, end1,end2;
	for (it1 = oldgreen.begin<uchar>(),it2=green.begin<uchar>(), end1 = oldgreen.end<uchar>(),end2=green.end<uchar>(); it1 != end1; ++it1,++it2)
	{
		*it2 = *it1 > 10 ? *it2 : 0;
	}
}

void seedgrow(Mat green)//0,0点为种子，>0点为边界，进行图像增长
{
	int v[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } };
	queue<struct point> q;
	struct point p1,p2;
	p1.x = 0, p1.y = 0;
	int x, y;
	q.push(p1);
	while (!q.empty())
	{
		p2=q.front();
		q.pop();
		for (int i = 0; i < 4; i++)
		{
			x = p2.x + v[i][0];
			y = p2.y + v[i][1];
			if (x<green.cols&&x>-1 && y<green.rows&&y>-1 && green.at<uchar>(y, x) == 0)
			{
				green.at<uchar>(y, x) = 255;
				p1.x = x, p1.y = y;
				q.push(p1);
			}
		}

	}
}
void killByBrightness(Mat ansgreen, Mat sourcegreen,Mat green1)//根据原图的亮度去掉亮度较小的exudate点，green为exudate的候选点，sourcegreen为原图像
{
	/*imshow("", green1);
	waitKey(0);*/
	Mat green = ansgreen.clone();


	ansgreen = Scalar(0);
	int max = 0;
	MatIterator_<uchar> it, end;
	for (it = sourcegreen.begin<uchar>(), end = sourcegreen.end<uchar>(); it != end; ++it)
	{
		max = *it > max ? *it : max;
	}
	max = 0.42*max;
	cout << max<<endl;
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } },x,y;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1,point2;
			if (green.at<uchar>(y, x) == 0)
			{
				green.at<uchar>(y, x) = 255;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) == 0)
						{
							green.at<uchar>(y, x) = 255;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}
				int tot = 0, below100 = 0;;
				for (auto begin1 = v.begin(), end = v.end(); begin1 != end; ++begin1)
				{
					tot += sourcegreen.at<uchar>((*begin1).y, (*begin1).x);
				}

				MatIterator_<uchar> it, end;
				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					if (green1.at<uchar>((*begin).y, (*begin).x)< 109) below100++;
					cout << (*begin).y << " " << (*begin).x <<" "<<(int)green1.at<uchar>((*begin).y, (*begin).x) << endl;
				}


				cout <<"v.size"<< v.size() << endl;
				cout <<"mean"<< tot / v.size() << endl;
				cout << "below:" << below100<<endl;
				if (below100<5)//gradientabove>4)//&&below100<5)//gradientabove>4)//tot / v.size() > max&&
				{
					for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
					{
						ansgreen.at<uchar>((*begin).y, (*begin).x)=255;
					}
				}
				/*imshow("", ansgreen);
				waitKey(0);
				imshow("g", green);
				waitKey(0);*/

			}
		}
	}
}

Mat getfinaleans(Mat img, Mat final)//将两张图片合成一张，并返回
{
	Mat ans(img.rows, img.cols * 2 + 5, img.type());
	for (int y = 0; y < ans.rows; y++)
	{
		for (int x = 0; x < ans.cols; x++)
		{
			if (x < img.cols)
			{
				ans.at<Vec3b>(y, x)[0] = img.at<Vec3b>(y, x)[0];
				ans.at<Vec3b>(y, x)[1] = img.at<Vec3b>(y, x)[1];
				ans.at<Vec3b>(y, x)[2] = img.at<Vec3b>(y, x)[2];
			}
			if (x >= img.cols&&x < img.cols + 5)
			{
				ans.at<Vec3b>(y, x)[0] = 255;
				ans.at<Vec3b>(y, x)[1] = 255;
				ans.at<Vec3b>(y, x)[1] = 255;
			}
			if (x>img.cols + 4)
			{
				if (final.type() == 16)
				{
					ans.at<Vec3b>(y, x)[0] = final.at<Vec3b>(y, x - img.cols - 5)[0];
					ans.at<Vec3b>(y, x)[1] = final.at<Vec3b>(y, x - img.cols - 5)[1];
					ans.at<Vec3b>(y, x)[2] = final.at<Vec3b>(y, x - img.cols - 5)[2];
				}
				else
				{
					ans.at<Vec3b>(y, x)[0] = final.at<uchar>(y, x - img.cols - 5);
					ans.at<Vec3b>(y, x)[1] = final.at<uchar>(y, x - img.cols - 5);
					ans.at<Vec3b>(y, x)[2] = final.at<uchar>(y, x - img.cols - 5);
				}

			}
		}
	}
	return ans;
}

bool cmp(struct point p1, struct point p2)
{
	if (p1.y < p2.y) return true;
	if (p1.y == p2.y&&p1.x < p2.x) return true;
	return false;
}
void cutIntoPieces(Mat markedimg,Mat img, Mat ansgreen, string des, int ii, bool over = false)//切割成小块，用于deep learning
{
	stringstream ss1;
	string pre;
	ss1 << ii;
	ss1 >> pre;
	/*FILE *fp = NULL;
	fopen_s(&fp, (des + pre).c_str(), "r");
	if (!fp)
	{
		cout << des + pre << endl;
		_mkdir((des + pre).c_str());
	}*/
	ifstream infile(des + "numans\\" + pre + ".txt");
	ofstream outfile(des + pre + "\\val.txt");
	Mat green = ansgreen.clone();
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	int index = 0;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1, point2;
			if (green.at<uchar>(y, x) >0)
			{
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) > 0)
						{
							green.at<uchar>(y, x) = 0;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}
				/*imshow("", green);
				waitKey(0);*/
				if (over)
				{
					int retain;
					infile >> retain;
					if (retain == 0)
					{
						for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
						{
							ansgreen.at<uchar>((*begin).y, (*begin).x) = 0;
						}
					}
					continue;
				}
				int leftx = v[0].x, lefty = v[0].y, rightx = v[0].x, righty = v[0].y;//找到最左面和最右边的点
				Mat temp = ansgreen.clone();//只标记处这次的小块
				temp = Scalar(0);
				sort(v.begin(), v.end(), cmp);
				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					if ((*begin).y < lefty) lefty = (*begin).y;
					if ((*begin).x<leftx) leftx = (*begin).x;
					if ((*begin).y > righty) righty = (*begin).y;
					if ((*begin).x >rightx) rightx = (*begin).x;
					temp.at<uchar>((*begin).y, (*begin).x) = 255;
				}

				//cout << lefttop.y << " " << lefttop.x <<" "<< rightbottom.y << " " << rightbottom.x << endl;
				/*leftx = leftx - 50 < 0 ? 0:leftx - 50;
				lefty = lefty - 50 < 0 ? 0 : lefty - 50;
				righty = righty + 50 >= img.rows ? img.rows - 1:righty + 50;
				rightx = rightx + 50 >= img.cols? img.cols - 1 : rightx + 50;*/
				int width = rightx - leftx + 1, high = righty - lefty + 1;
				int threshold = 15;
				int beilv = 3;
				if (width < threshold) width = threshold;
				if (high < threshold) high = threshold;
				int side = width > high ? width : high;
				side = side;
				lefty = v[v.size()/2].y - beilv*side ;
				leftx = v[v.size()/2].x - beilv*side ;
				/*if ((leftx - beilv * width)< 0 || rightx + beilv * width >= img.cols)
				{
				int tempwidth = leftx < img.cols - 1 - rightx ? leftx : img.cols - 1 - rightx;
				leftx = leftx - tempwidth;
				rightx = rightx + tempwidth;
				}
				else
				{
				leftx = leftx - beilv * width;
				rightx = rightx + beilv * width;
				}
				if (lefty - beilv * high < 0 || righty + beilv * high >= img.rows)
				{
				int temphigh = lefty < img.rows - 1 - righty ? lefty : img.rows - 1 - righty;
				lefty = lefty - temphigh;
				righty = righty + temphigh;
				}
				else
				{
				lefty = lefty - beilv * high;
				righty = righty + beilv * high;
				}*/

				Mat file(side*(2 * beilv), side*(2 * beilv), img.type());
				Mat file2(side*(2 * beilv), side*(2 * beilv), img.type());
				for (int filey = 0; filey <side*(2 * beilv); filey++)
				{
					for (int filex = 0; filex <side*(2 * beilv); filex++)
					{
						int tempy = filey + lefty, tempx = filex + leftx;
						//cout << filey + lefty - 10 << " " << filex + leftx - 10 << endl;
						if (filey + lefty < img.rows&&filex + leftx < img.cols&&filey + lefty >= 0 && filex + leftx >= 0)
						{
							file.at<Vec3b>(filey, filex)[0] = img.at<Vec3b>(filey + lefty, filex + leftx)[0];
							file.at<Vec3b>(filey, filex)[1] = img.at<Vec3b>(filey + lefty, filex + leftx)[1];
							file.at<Vec3b>(filey, filex)[2] = img.at<Vec3b>(filey + lefty, filex + leftx)[2];
						}
						else
						{

							if (filey + lefty >= img.rows)	tempy = img.rows - 1 - (filey + lefty - img.rows + 1);
							if (filex + leftx >= img.cols)	tempx = img.cols - 1 - (filex + leftx - img.cols + 1);
							if (filey + lefty < 0) tempy = 0 - tempy;
							if (filex + leftx < 0)	tempx = 0 - tempx;
							/*	if (tempy < 0 || tempx < 0 || tempy >= img.rows || tempx >= img.cols)
							{
							int n=0;
							tempy++;
							}*/
							file.at<Vec3b>(filey, filex)[0] = img.at<Vec3b>(tempy, tempx)[0];
							file.at<Vec3b>(filey, filex)[1] = img.at<Vec3b>(tempy, tempx)[1];
							file.at<Vec3b>(filey, filex)[2] = img.at<Vec3b>(tempy, tempx)[2];
						}

						if (temp.at<uchar>(tempy, tempx) > 0 && !(markedimg.at<Vec3b>(tempy, tempx)[2]>230 && markedimg.at<Vec3b>(tempy, tempx)[1]>120))
						{
							file2.at<Vec3b>(filey, filex)[0] = temp.at<uchar>(tempy, tempx);
							file2.at<Vec3b>(filey, filex)[1] = temp.at<uchar>(tempy, tempx);
							file2.at<Vec3b>(filey, filex)[2] = temp.at<uchar>(tempy, tempx);
						}
						else
						{
							file2.at<Vec3b>(filey, filex)[0] = markedimg.at<Vec3b>(tempy, tempx)[0];
							file2.at<Vec3b>(filey, filex)[1] = markedimg.at<Vec3b>(tempy, tempx)[1];
							file2.at<Vec3b>(filey, filex)[2] = markedimg.at<Vec3b>(tempy, tempx)[2];
						}

					}
				}
				stringstream ss;
				ss << index;
				string indexnum;
				ss >> indexnum;
				//file = getfinaleans(file, file2);
				imwrite(des + pre + "\\" + pre + "_" + indexnum + ".jpg", file);
				/*namedWindow(indexnum, WINDOW_NORMAL);
				resizeWindow(indexnum, 1000, 570);
				moveWindow(indexnum, 150, 50);
				imshow(indexnum, file);
				char returnkey;
				returnkey = waitKey(0);
				destroyWindow(indexnum);
				outfile << indexnum + ".jpg " <<returnkey<< "\n";
				outfile.flush();*/
				index++;
				/*for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
				green.at<uchar>((*begin).y, (*begin).x) = 0;
				}
				*/

			}
		}
	}
}


int totnum = 0;
ofstream fileout("C:\\Users\\ljpadamwin\\Desktop\\DR\\train\\train.txt");
void cutIntoAllPieces(Mat img, Mat ansgreen, string des, int ii, bool over = false)//切割完所有小块，按像素切割；用于deep learning
{
	stringstream ss1;
	string pre;
	ss1 << ii;
	ss1 >> pre;
	/*FILE *fp = NULL;
	fopen_s(&fp, (des + pre).c_str(), "r");
	if (!fp)
	{
		cout << des + pre << endl;
		_mkdir((des + pre).c_str());
	}*/
	Mat green = ansgreen.clone();
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	int index = 0;
	ifstream filein("C:\\Users\\ljpadamwin\\Desktop\\DR\\ans\\over412\\" + pre + ".txt");
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1, point2;
			if (green.at<uchar>(y, x) >0)
			{
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) > 0)
						{
							green.at<uchar>(y, x) = 0;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}
				int leftx = v[0].x, lefty = v[0].y, rightx = v[0].x, righty = v[0].y;//找到最左面和最右边的点
				Mat temp = ansgreen.clone();//只标记处这次的小块
				temp = Scalar(0);
				string key, value;
				filein >> key >> value;

				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					if ((*begin).y < lefty) lefty = (*begin).y;
					if ((*begin).x<leftx) leftx = (*begin).x;
					if ((*begin).y > righty) righty = (*begin).y;
					if ((*begin).x >rightx) rightx = (*begin).x;
					temp.at<uchar>((*begin).y, (*begin).x) = 255;
				}
				int width = rightx - leftx + 1, high = righty - lefty + 1;
				int threshold = 15;
				int beilv = 3;
				if (width < threshold) width = threshold;
				if (high < threshold) high = threshold;
				int side = width > high ? width : high;
				side = side;


				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					lefty = (*begin).y;
					leftx = (*begin).x;
					lefty = lefty - beilv*side;
					leftx = leftx - beilv*side;
					Mat file(side*(2 * beilv)+1, side*(2 * beilv)+1, img.type());
					Mat file2(side*(2 * beilv)+1, side*(2 * beilv)+1, img.type());
					for (int filey = 0; filey <side*(2 * beilv)+1; filey++)
					{
						for (int filex = 0; filex <side*(2 * beilv)+1; filex++)
						{
							//cout << filey + lefty - 10 << " " << filex + leftx - 10 << endl;
							if (filey + lefty < img.rows&&filex + leftx < img.cols&&filey + lefty >= 0 && filex + leftx >= 0)
							{
								file.at<Vec3b>(filey, filex)[0] = img.at<Vec3b>(filey + lefty, filex + leftx)[0];
								file.at<Vec3b>(filey, filex)[1] = img.at<Vec3b>(filey + lefty, filex + leftx)[1];
								file.at<Vec3b>(filey, filex)[2] = img.at<Vec3b>(filey + lefty, filex + leftx)[2];
							}
							else
							{
								int tempy = filey + lefty, tempx = filex + leftx;
								if (filey + lefty >= img.rows)	tempy = img.rows - 1 - (filey + lefty - img.rows + 1);
								if (filex + leftx >= img.cols)	tempx = img.cols - 1 - (filex + leftx - img.cols + 1);
								if (filey + lefty < 0) tempy = 0 - tempy;
								if (filex + leftx < 0)	tempx = 0 - tempx;
								file.at<Vec3b>(filey, filex)[0] = img.at<Vec3b>(tempy, tempx)[0];
								file.at<Vec3b>(filey, filex)[1] = img.at<Vec3b>(tempy, tempx)[1];
								file.at<Vec3b>(filey, filex)[2] = img.at<Vec3b>(tempy, tempx)[2];
							}
						}
					}
					stringstream totss;
					totss << totnum;
					string totnumstring;
					totss >> totnumstring;
					imwrite("C:\\Users\\ljpadamwin\\Desktop\\DR\\train\\" + totnumstring + ".jpg", file);
					fileout << totnumstring + ".jpg " + value + "\n";
					totnum++;
				}

				index++;

			}
		}
	}
}


Mat average_killbyneigbor(Mat img, Mat ansgreen,double percent=0.7)//img,原始的绿色通道，ansgreen候选块,如果该块小于周围块的亮度的百分之多少才留下
{

	Mat green = ansgreen.clone();
	Mat ans = ansgreen.clone();
	ans = Scalar(0);
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	int index = 0;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1, point2;
			if (green.at<uchar>(y, x) >0)
			{
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) > 0)
						{
							green.at<uchar>(y, x) = 0;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}

				double thissum = 0, thisnum = 0,neigbursum=0,neigbornum=0;
				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					thissum += img.at<uchar>((*begin).y, (*begin).x);
					thisnum++;
				}
				int leftx = v[0].x, lefty = v[0].y, rightx = v[0].x, righty = v[0].y;//找到最左面和最右边的点
				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					if ((*begin).y < lefty) lefty = (*begin).y;
					if ((*begin).x<leftx) leftx = (*begin).x;
					if ((*begin).y > righty) righty = (*begin).y;
					if ((*begin).x >rightx) rightx = (*begin).x;
				}
				int width = rightx - leftx + 1, high = righty - lefty + 1;
				int threshold = 5;
				int beilv = 2;
				if (width < threshold) width = threshold;
				if (high < threshold) high = threshold;
				if ((leftx - beilv * width)< 0 || rightx + beilv * width >= img.cols)
				{
					int tempwidth = leftx < img.cols - 1 - rightx ? leftx : img.cols - 1 - rightx;
					leftx = leftx - tempwidth;
					rightx = rightx + tempwidth;
				}
				else
				{
					leftx = leftx - beilv * width;
					rightx = rightx + beilv * width;
				}
				if (lefty - beilv * high < 0 || righty + beilv * high >= img.rows)
				{
					int temphigh = lefty < img.rows - 1 - righty ? lefty : img.rows - 1 - righty;
					lefty = lefty - temphigh;
					righty = righty + temphigh;
				}
				else
				{
					lefty = lefty - beilv * high;
					righty = righty + beilv * high;
				}
				int temppix;
				for (int filey = 0; filey <righty - lefty + 1; filey++)
				{
					for (int filex = 0; filex <rightx - leftx + 1; filex++)
					{
						temppix = img.at<uchar>(filey + lefty, filex + leftx);
						if (temppix < 5) continue;
						neigbursum += temppix;
						neigbornum++;
					}
				}
				if (thissum / thisnum < neigbursum / neigbornum-percent)
				{
					for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
					{
						ans.at<uchar>((*begin).y, (*begin).x) = 255;
					}
				}

			}
		}
	}
	return ans;
}

Mat killbyneigbor(Mat img, Mat ansgreen, double percent = 0.7)//img,原始的绿色通道，ansgreen候选块,如果该块小于周围块的亮度的百分之多少才留下
{

	Mat green = ansgreen.clone();
	Mat ans = ansgreen.clone();
	ans = Scalar(0);
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	int index = 0;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1, point2;
			if (green.at<uchar>(y, x) >0)
			{
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) > 0)
						{
							green.at<uchar>(y, x) = 0;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}

				double thissum = 0, thisnum = 0, neigbursum = 0, neigbornum = 0;
				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					thissum += img.at<uchar>((*begin).y, (*begin).x);
					thisnum++;
				}
				int leftx = v[0].x, lefty = v[0].y, rightx = v[0].x, righty = v[0].y;//找到最左面和最右边的点
				for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
				{
					if ((*begin).y < lefty) lefty = (*begin).y;
					if ((*begin).x<leftx) leftx = (*begin).x;
					if ((*begin).y > righty) righty = (*begin).y;
					if ((*begin).x >rightx) rightx = (*begin).x;
				}
				int width = rightx - leftx + 1, high = righty - lefty + 1;
				int threshold = 5;
				int beilv = 2;
				if (width < threshold) width = threshold;
				if (high < threshold) high = threshold;
				if ((leftx - beilv * width)< 0 || rightx + beilv * width >= img.cols)
				{
					int tempwidth = leftx < img.cols - 1 - rightx ? leftx : img.cols - 1 - rightx;
					leftx = leftx - tempwidth;
					rightx = rightx + tempwidth;
				}
				else
				{
					leftx = leftx - beilv * width;
					rightx = rightx + beilv * width;
				}
				if (lefty - beilv * high < 0 || righty + beilv * high >= img.rows)
				{
					int temphigh = lefty < img.rows - 1 - righty ? lefty : img.rows - 1 - righty;
					lefty = lefty - temphigh;
					righty = righty + temphigh;
				}
				else
				{
					lefty = lefty - beilv * high;
					righty = righty + beilv * high;
				}
				int temppix;
				for (int filey = 0; filey <righty - lefty + 1; filey++)
				{
					for (int filex = 0; filex <rightx - leftx + 1; filex++)
					{
						temppix = img.at<uchar>(filey + lefty, filex + leftx);
						if (temppix < 5) continue;
						neigbursum += temppix;
						neigbornum++;
					}
				}
				//cout << thissum / thisnum << " " << neigbursum / neigbornum << endl;
				if (thissum / thisnum < neigbursum / neigbornum - percent)
				{
					for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
					{
						ans.at<uchar>((*begin).y, (*begin).x) = 255;
					}
				}

			}
		}
	}
	return ans;
}

void killByGradient(Mat ansgreen, Mat sourcegreen, Mat green1)//根据原图的亮度去掉亮度较小的exudate点，green为exudate的候选点，sourcegreen为原图像
{
	/*imshow("", green1);
	waitKey(0);*/
	const int top = 50;
	Mat green = ansgreen.clone();
	Mat gradient = sourcegreen.clone();

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32F;
	/// Gradient X
	Sobel(gradient, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	/// Gradient Y
	Sobel(gradient, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	/*convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);*/

	gradient = grad_x.clone();
	//std::cout <<   (gradient.depth == CV_32F || gradient.depth == CV_64F) << endl;

	try
	{
		magnitude(grad_x, grad_y, gradient);
	}
	catch (Exception ex)
	{
		std::cout << ex.msg << endl;
	}
	convertScaleAbs(gradient, gradient);//转化一下，原来是32F，转化为uchar
	/*imshow("g", gradient);
	waitKey(0);*/

	ansgreen = Scalar(0);
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			//cout << "h:"<<y<<" " << x << endl;
			vector<struct point> v;
			queue<struct point> q;
			int gradientabove = 0;//记录梯度值大于3的点的个数
			struct point point1, point2;
			if (green.at<uchar>(y, x) == 255)
			{
				cout << y << " " << x << endl;
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				if (gradient.at<uchar>(y, x) > top) gradientabove++;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) == 255)
						{
							green.at<uchar>(y, x) = 0;
							if (gradient.at<uchar>(y, x) > top) gradientabove++;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}

				cout << "gradient:" << gradientabove << endl;
				if (gradientabove>3)//gradientabove>4)//&&below100<5)//gradientabove>4)//tot / v.size() > max&&
				{
					for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
					{
						ansgreen.at<uchar>((*begin).y, (*begin).x) = 255;
					}
				}
				/*imshow("", ansgreen);
				waitKey(0);
				imshow("g", green);
				waitKey(0);*/

			}
		}
	}
}


void killByDifferenceBelowsource(Mat ansgreen, Mat sourcegreen)//取消差值小于原图百分之几的像素
{

	Mat green = ansgreen.clone();

	ansgreen = Scalar(0);
	double belowpercent = 0.08;
	int pixel,totabove=0;
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1, point2;
			if (x>-1 && x < green.cols&&y>-1 && y < green.rows)
			{
				pixel = green.at<uchar>(y, x);
			}
			double sum = 0;
			if (green.at<uchar>(y, x)>0)
			{
				totabove = 0;
				sum += (double)pixel / (double)sourcegreen.at<uchar>(y, x);
				if (pixel>belowpercent*sourcegreen.at<uchar>(y, x)) totabove++;
				cout << pixel << " "<<belowpercent*sourcegreen.at<uchar>(y, x) << endl;
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows)
						{
							pixel = green.at<uchar>(y, x);
						}

						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&pixel > 0)
						{
							sum += (double)pixel / (double)sourcegreen.at<uchar>(y, x);
							if (pixel>belowpercent*sourcegreen.at<uchar>(y, x)) totabove++;
							green.at<uchar>(y, x) = 0;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}
				cout << "totabove:" << totabove;
				cout << " size:" << v.size()<<endl;
				cout << "average:" << sum / v.size() <<"size"<<v.size()<< endl;
				if (totabove>0.8*v.size())//gradientabove>4)//&&below100<5)//gradientabove>4)//tot / v.size() > max&&
				{
					for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
					{
						ansgreen.at<uchar>((*begin).y, (*begin).x) = 120;
					}
				}
				imshow("", green);
				waitKey(0);
				//imshow("ans", ansgreen);
			}
		}
	}
}


Mat killByElementSize(Mat ogreen, int size,bool above)//取消面积小于或大于size的，above为true为大于
{
	Mat ansgreen = ogreen.clone();
	Mat green = ansgreen.clone();

	ansgreen = Scalar(0);
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1, point2;
			if (green.at<uchar>(y, x)>0)
			{
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) > 0)
						{
							green.at<uchar>(y, x) = 0;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}
				if ((above&&v.size()>size)||(!above&&v.size()<size))//gradientabove>4)//&&below100<5)//gradientabove>4)//tot / v.size() > max&&
				{
					for (auto begin = v.begin(), end = v.end(); begin != end; ++begin)
					{
						ansgreen.at<uchar>((*begin).y, (*begin).x) = 255;
					}
				}
			}
		}
	}
	return ansgreen;
}

int average(Mat green)
{
	MatIterator_<uchar> begin, end;
	int sum = 0, totnum = 0;
	for (begin = green.begin<uchar>(), end = green.end<uchar>(); begin != end; ++begin)
	{
		if (*begin > 25)
		{
			totnum++;
			sum += *begin;
		}
	}
	return sum / totnum;
}
void complete(Mat green)//255变0,0变255
{
	MatIterator_<uchar> begin, end;
	int sum = 0, totnum = 0;
	for (begin = green.begin<uchar>(), end = green.end<uchar>(); begin != end; ++begin)
	{
		*begin = 255 - *begin;
	}
}
void belowplus(Mat green, int below, int plus)
{
	MatIterator_<uchar> begin, end;
	for (begin = green.begin<uchar>(), end = green.end<uchar>(); begin != end; ++begin)
	{
		if (*begin > 25 && *begin < below)
		{
			*begin = 255;
		}
		else
			*begin = 0;
	}
}
void cutbelow(Mat green, int below)
{
	MatIterator_<uchar> begin, end;
	for (begin = green.begin<uchar>(), end = green.end<uchar>(); begin != end; ++begin)
	{
		if (*begin < below) *begin = 0;
	}
}
void cutbyblue(Mat green, Mat blue)
{
	for (int y = 0; y < green.rows; y++)
	{
		for (int x = 0; x < green.cols; x++)
		{
			int temp = (int)green.at<uchar>(y, x) - (int)blue.at<uchar>(y, x) + 40;
			if (temp < 0) temp = 0;
			if (temp > 255) temp = 255;
			green.at<uchar>(y, x) = temp;
		}
	}
}

bool excudatetrue(Mat green145)
{
	MatIterator_<uchar> begin, end;
	for (begin = green145.begin<uchar>(), end = green145.end<uchar>(); begin != end; ++begin)
	{
		if (*begin ==255) return true;
	}
	return false;
}
void showHistogram(const Mat green)
{
	/// 设定bin数目
	int histSize = 255;

	/// 设定取值范围 ( R,G,B) )
	float range[] = { 0, 255 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;
	Mat g_hist;
	calcHist(&green, 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	int hist_w = 400; int hist_h = 800;
	int bin_w = cvRound((double)hist_h / histSize);
	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++)
	{

		line(histImage, Point(bin_w*(i - 1), hist_w - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_w - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
	}
	imshow("calcHist Demo", histImage);
	waitKey(0);



}
int maxValue(Mat a)
{
	int max = 0;
	MatIterator_<uchar> begin, end;
	for (begin =a.begin<uchar>(), end = a.end<uchar>(); begin != end; ++begin)
	{
		if (*begin>max) max = *begin;
	}
	return max;
}

Mat getMarkedImg(Mat img)
{

		vector<Mat> channels;
		split(img, channels);
		Mat green = channels.at(1);
		Mat source = green.clone();
		Mat source2 = green.clone();


		Mat a1=green.clone(), a2=green.clone();
		medianBlur(a1, a1, 3);
		medianBlur(a2, a2, 3);
		Mat b1 = getStructuringElement(MORPH_ELLIPSE, Size(8, 8));//核的大小
		morphologyEx(a1, a1, CV_MOP_CLOSE, b1);
		Mat b2 = getStructuringElement(MORPH_ELLIPSE, Size(1, 1));//核的大小
		morphologyEx(a2, a2, CV_MOP_CLOSE, b2);
		a1 = matsubtract(a1, a2);
		//int max1=0.1*maxValue(a1);
		aboveplus(a1, 2, 0);//求血管
		Mat b3 = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));//核的大小
		morphologyEx(a1, a1, CV_MOP_CLOSE, b3);
		Mat b4 = getStructuringElement(MORPH_ELLIPSE, Size(1, 1));//核的大小
		morphologyEx(a1, a1, CV_MOP_DILATE, b4);
		Mat e1 = killByElementSize(a1, 500, true);

		a1 = matsubtract(a1, e1);
		aboveplus(a1, 3, 125);//4.10 =2
		Mat tempgreen = channels.at(1);
		markans(tempgreen, e1,0);
		a1 = killbyneigbor(tempgreen, a1, 4);
		killout(source, a1);
		return a1;
		//cutIntoPieces(markedimg,img, a1, des, ii,true);
		//markans(img, a1);
		//Mat finalans = getfinaleans(img, a1);
		//imwrite(desimg, finalans);
}
void cutIntoPiecesForLearning(Mat img, Mat ansgreen)
{
	Mat green = ansgreen.clone();
	Mat ans=ansgreen.clone();
	ans = Scalar(0);
	int direction[4][2] = { { -1, 0 }, { 1, 0 }, { 0, 1 }, { 0, -1 } }, x, y;
	int index = 0;
	for (int yy = 0; yy < green.rows; yy++)
	{
		for (int xx = 0; xx < green.cols; xx++)
		{
			int y = yy, x = xx;
			vector<struct point> v;
			queue<struct point> q;
			struct point point1, point2;
			if (green.at<uchar>(y, x) >0)
			{
				green.at<uchar>(y, x) = 0;
				point1.x = x, point1.y = y;
				q.push(point1);
				while (!q.empty())
				{
					point1 = q.front();
					q.pop();
					v.push_back(point1);

					for (int i = 0; i < 4; i++)
					{
						x = point1.x + direction[i][0];
						y = point1.y + direction[i][1];
						if (x>-1 && x < green.cols&&y>-1 && y < green.rows&&green.at<uchar>(y, x) > 0)
						{
							green.at<uchar>(y, x) = 0;
							point2.x = x;
							point2.y = y;
							q.push(point2);
						}
					}
				}

				int leftx = v[0].x, lefty = v[0].y, rightx = v[0].x, righty = v[0].y;//找到最左面和最右边的点
				int width = rightx - leftx + 1, high = righty - lefty + 1;
				int threshold = 15;
				int beilv = 3;
				if (width < threshold) width = threshold;
				if (high < threshold) high = threshold;
				int side = width > high ? width : high;
				side = side;
				lefty = v[v.size()/2].y - beilv*side ;
				leftx = v[v.size()/2].x - beilv*side ;

				Mat file(side*(2 * beilv), side*(2 * beilv), img.type());
				for (int filey = 0; filey <side*(2 * beilv); filey++)
				{
					for (int filex = 0; filex <side*(2 * beilv); filex++)
					{
						int tempy = filey + lefty, tempx = filex + leftx;
						//cout << filey + lefty - 10 << " " << filex + leftx - 10 << endl;
						if (filey + lefty < img.rows&&filex + leftx < img.cols&&filey + lefty >= 0 && filex + leftx >= 0)
						{
							file.at<Vec3b>(filey, filex)[0] = img.at<Vec3b>(filey + lefty, filex + leftx)[0];
							file.at<Vec3b>(filey, filex)[1] = img.at<Vec3b>(filey + lefty, filex + leftx)[1];
							file.at<Vec3b>(filey, filex)[2] = img.at<Vec3b>(filey + lefty, filex + leftx)[2];
						}
						else
						{

							if (filey + lefty >= img.rows)	tempy = img.rows - 1 - (filey + lefty - img.rows + 1);
							if (filex + leftx >= img.cols)	tempx = img.cols - 1 - (filex + leftx - img.cols + 1);
							if (filey + lefty < 0) tempy = 0 - tempy;
							if (filex + leftx < 0)	tempx = 0 - tempx;
							file.at<Vec3b>(filey, filex)[0] = img.at<Vec3b>(tempy, tempx)[0];
							file.at<Vec3b>(filey, filex)[1] = img.at<Vec3b>(tempy, tempx)[1];
							file.at<Vec3b>(filey, filex)[2] = img.at<Vec3b>(tempy, tempx)[2];
						}

					}
				}
				//imwrite("/home/ljpadam/Desktop/test/1/"+inttostring(index)+".jpg",file);

				index++;
			}
		}
	}
}

void JPPreHamor(Mat src, Mat& result)
{
	int rows=src.rows,cols=src.cols;
    src=letresize(src);

	result=getMarkedImg(src);

	resize(result, result, Size(cols, rows));//ans is the highlight answer

}


