// SURF.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#if 0
//#include <opencv2/opencv.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/legacy/legacy.hpp" 


using namespace cv;
using namespace std;

//主函数
int main()
{
	//载入源图像，显示并转换灰度图
	Mat srcImage = imread("C:\\Users\\mrxu\\Desktop\\aa.jpg"), grayImage;
	namedWindow("image[origin]", 1);
	imshow("image[origin]", srcImage);
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

	//检测surf关键点、提取训练图像描述符
	vector<KeyPoint> keyPoint;
	Mat descriptor;
	SurfFeatureDetector featureDetector(80);
	featureDetector.detect(grayImage, keyPoint);
	SurfDescriptorExtractor featureExtractor;
	featureExtractor.compute(grayImage, keyPoint, descriptor);

	//创建基于FLANN的描述符匹配对象
	FlannBasedMatcher matcher;
	vector<Mat> desc_collection(1, descriptor);
	matcher.add(desc_collection);
	matcher.train();

	//创建视频对象、定义帧率
	VideoCapture cap(0);
	unsigned int frameCount = 0;//帧数

	//不断循环，直到q键被按下
	while (char(waitKey(1)) != 27)
	{
		//参数设置
		int64 time0 = getTickCount();
		Mat testImage, grayImage_test;
		cap >> testImage;//采集视频到testImage中
		if (testImage.empty())
			continue;
		//转换图像到灰度
		cvtColor(testImage, grayImage_test, COLOR_BGR2GRAY);
		//检测S关键点、提取测试图像描述符
		vector<KeyPoint> keyPoint_test;
		Mat descriptor_test;
		featureDetector.detect(grayImage_test, keyPoint_test);
		featureExtractor.compute(grayImage_test, keyPoint_test, descriptor_test);

		//匹配训练和测试描述符
		vector<vector<DMatch>>matches;
		matcher.knnMatch(descriptor_test, matches, 2);
		//根据劳氏算法，得到优秀的匹配点
		vector<DMatch> goodMatches;
		for (unsigned int i = 0; i<matches.size(); ++i)
		{
			if (matches[i][0].distance<0.6*matches[i][1].distance)
				goodMatches.push_back(matches[i][0]);
		}
		//绘制匹配点并显示窗口
		Mat dstImage;
		drawMatches(testImage, keyPoint_test, srcImage, keyPoint, goodMatches, dstImage);
		namedWindow("image[match]", 1);
		imshow("image[match]", dstImage);

		//输出帧率信息
		cout << "当前帧率为：" << getTickFrequency() / (getTickCount() - time0) << endl;
	}
	return 0;
}
#endif