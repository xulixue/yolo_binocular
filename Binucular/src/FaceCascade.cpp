#include "stdafx.h"
#include "FaceCascade.h"

using namespace std;
using namespace cv;

FaceCascade::FaceCascade(string path)
{
	cascade.load(path);

}

FaceCascade::~FaceCascade()
{
}

vector<Rect> FaceCascade::Detect(Mat &srcImg){
	cv::Mat dstImg, grayImg;
    resize(srcImg, srcImg, Size(srcImg.cols / 1, srcImg.rows / 1), 0, 0, INTER_LINEAR);
	dstImg = srcImg.clone();
    imshow("detecOrg", srcImg);
	//waitKey(0);
	grayImg.create(srcImg.size(), srcImg.type());
    cvtColor(srcImg, grayImg, CV_BGR2GRAY);

	Scalar colors[] =
	{

		CV_RGB(255, 0, 0),
		CV_RGB(255, 97, 0),
		CV_RGB(255, 255, 0),
		CV_RGB(0, 255, 0),
		CV_RGB(255, 97, 0),
		CV_RGB(0, 0, 255),
		CV_RGB(160, 32, 240),
	};

	vector<Rect> rect;
    cascade.detectMultiScale(grayImg, rect, 1.1, 3, 0);
    cout << "rect.size():" << rect.size() << endl;
	for (int i = 0; i < rect.size(); i++)
	{
		Point center;
		int radius;
		center.x = cvRound((rect[i].x + rect[i].width * 0.5));
		center.y = cvRound((rect[i].y + rect[i].height * 0.5));
		radius = cvRound((rect[i].width + rect[i].height) *0.25);
		circle(dstImg, center, radius, colors[i % 7], 2);
	}
    imshow("dstImg", dstImg);
    waitKey(10);
	return rect;

}
