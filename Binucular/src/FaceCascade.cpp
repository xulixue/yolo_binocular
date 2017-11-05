#include "stdafx.h"
#include "FaceCascade.h"

using namespace std;
using namespace cv;

FaceCascade::FaceCascade(string path)
{
	//1.加载分类器
	//如果要识别人体的其它部位，只需将上面的haarcascade_frontalface_alt2.xml分类器替换即可。
	cascade.load(path);

}

FaceCascade::~FaceCascade()
{
}

vector<Rect> FaceCascade::Detect(Mat &srcImg){
	cv::Mat dstImg, grayImg;
	//尺寸调整  
	resize(srcImg, srcImg, Size(srcImg.cols / 1, srcImg.rows / 1), 0, 0, INTER_LINEAR); //用线性插值
	dstImg = srcImg.clone();
//	imshow("原图", srcImg);
    imshow("detecOrg", srcImg);
	//waitKey(0);
	grayImg.create(srcImg.size(), srcImg.type());
	cvtColor(srcImg, grayImg, CV_BGR2GRAY);//生成灰度图，提高检测效率

	//定义7种颜色，用于标记人脸
	Scalar colors[] =
	{
		// 红橙黄绿青蓝紫
		CV_RGB(255, 0, 0),
		CV_RGB(255, 97, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 97, 0),
		CV_RGB(0, 0, 255),
		CV_RGB(160, 32, 240),
	};
	// 3.检测
	vector<Rect> rect;
	cascade.detectMultiScale(grayImg, rect, 1.1, 3, 0);//分类器对象调用

    printf("rect.size():%d\n", rect.size());

	//4.标记--在脸部画圆
	for (int i = 0; i < rect.size(); i++)
	{
		Point center;
		int radius;
		center.x = cvRound((rect[i].x + rect[i].width * 0.5));
		center.y = cvRound((rect[i].y + rect[i].height * 0.5));
		radius = cvRound((rect[i].width + rect[i].height) *0.25);
		circle(dstImg, center, radius, colors[i % 7], 2);
	}
	//5.显示
//	imshow("识别结果", dstImg);
    imshow("sdf", dstImg);
    waitKey(10);
	return rect;

}
