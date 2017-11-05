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
#include <opencv2/objdetect/objdetect.hpp>
// #include <opencv2/features2d/features2d.hpp>
// #include <opencv2/nonfree/nonfree.hpp>
// #include <opencv2/nonfree/features2d.hpp>
#endif

class FaceCascade
{
public:
	FaceCascade(std::string path);
	~FaceCascade();
	std::vector<cv::Rect> Detect(cv::Mat &srcImg);

    cv::CascadeClassifier cascade;
private:


};
