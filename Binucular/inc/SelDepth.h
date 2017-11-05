#include "stdafx.h"
#pragma once
#include <iostream>

#ifdef WIN32
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#else
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/features2d/features2d.hpp>
// #include "/usr/local/opencv2.4.13/include/opencv2/nonfree/features2d.hpp"
// #include "/usr/local/opencv2.4.13/include/opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#endif

struct SelBox
{
	int lft;
	int rit;
	int top;
	int botm;
	std::string name;
	int ret_x2_de_x1;					//���ڴ��ŷ��ص�x2 - x1ֵ��    -1˵������û��������
	int deepth_mm;
	std::vector<cv::DMatch> selMatch;			//��������������ƥ���ԡ�
};

class SelDepth
{
public:
	SelDepth();
	~SelDepth();
	//	vector<SelBox> selbox;			//��¶�Ŀ����ⲿѡ��
	void RunSURF(cv::Mat &imgL, cv::Mat &imgR);
	void SelMatchEach(SelBox &sb, bool showDebug);
	void ShowSelMatch(cv::Mat &imgL, cv::Mat &imgR);
	void ShowAllSelMatch(cv::Mat &imgL, cv::Mat &imgR);
	void SelMatch(std::vector<SelBox> &sbs);
	void GetAllDeepth(std::vector<SelBox> &sbs);
	void Clear();

private:
	std::vector<cv::DMatch>matches;							//���е�����ƥ���ԡ�
	std::vector<cv::DMatch>selAllMatches;					//���еľ���ɸѡ��ƥ���ԡ�
	std::vector<cv::KeyPoint>keypoint1, keypoint2;           //��������KeyPoint����ʸ���洢���⵽�������㣬���е��������Ҳࡣ

	bool InsideROI(int x, int y, SelBox sb);
	void GetAllSelMatch(std::vector<SelBox> &sbs);
	//void GetEachDeepth(SelBox &sb);
	// Mat imgL, imgR;

};

#define OFFSET_VERTICAL_Y2_Y1			(13)			//y2-y1
#define OFFSET_VERTICAL_Y2_Y1_PERMIT	3			//�ݲ���3
