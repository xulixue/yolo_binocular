// SURF.cpp : �������̨Ӧ�ó������ڵ㡣
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

//������
int main()
{
	//����Դͼ����ʾ��ת���Ҷ�ͼ
	Mat srcImage = imread("C:\\Users\\mrxu\\Desktop\\aa.jpg"), grayImage;
	namedWindow("image[origin]", 1);
	imshow("image[origin]", srcImage);
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

	//���surf�ؼ��㡢��ȡѵ��ͼ��������
	vector<KeyPoint> keyPoint;
	Mat descriptor;
	SurfFeatureDetector featureDetector(80);
	featureDetector.detect(grayImage, keyPoint);
	SurfDescriptorExtractor featureExtractor;
	featureExtractor.compute(grayImage, keyPoint, descriptor);

	//��������FLANN��������ƥ�����
	FlannBasedMatcher matcher;
	vector<Mat> desc_collection(1, descriptor);
	matcher.add(desc_collection);
	matcher.train();

	//������Ƶ���󡢶���֡��
	VideoCapture cap(0);
	unsigned int frameCount = 0;//֡��

	//����ѭ����ֱ��q��������
	while (char(waitKey(1)) != 27)
	{
		//��������
		int64 time0 = getTickCount();
		Mat testImage, grayImage_test;
		cap >> testImage;//�ɼ���Ƶ��testImage��
		if (testImage.empty())
			continue;
		//ת��ͼ�񵽻Ҷ�
		cvtColor(testImage, grayImage_test, COLOR_BGR2GRAY);
		//���S�ؼ��㡢��ȡ����ͼ��������
		vector<KeyPoint> keyPoint_test;
		Mat descriptor_test;
		featureDetector.detect(grayImage_test, keyPoint_test);
		featureExtractor.compute(grayImage_test, keyPoint_test, descriptor_test);

		//ƥ��ѵ���Ͳ���������
		vector<vector<DMatch>>matches;
		matcher.knnMatch(descriptor_test, matches, 2);
		//���������㷨���õ������ƥ���
		vector<DMatch> goodMatches;
		for (unsigned int i = 0; i<matches.size(); ++i)
		{
			if (matches[i][0].distance<0.6*matches[i][1].distance)
				goodMatches.push_back(matches[i][0]);
		}
		//����ƥ��㲢��ʾ����
		Mat dstImage;
		drawMatches(testImage, keyPoint_test, srcImage, keyPoint, goodMatches, dstImage);
		namedWindow("image[match]", 1);
		imshow("image[match]", dstImage);

		//���֡����Ϣ
		cout << "��ǰ֡��Ϊ��" << getTickFrequency() / (getTickCount() - time0) << endl;
	}
	return 0;
}
#endif