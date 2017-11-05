
#include "stdafx.h"
#if 0
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
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#endif
#include <time.h>
#include <vector>

#include "SelDepth.h"
#include "FaceCascade.h"

using namespace std;
using namespace cv;

clock_t startTime, endTime;

//void SelDeep::GetEachDeepth(SelBox &sb){
//
//}

Mat srcImg;
int main()
{
	VideoCapture capL(0);
    FaceCascade fc("//home//mrxu//Desktop//haarcascade_frontalface_alt2.xml");
	while (true)
	{
#if 0
		//2.¶ÁÍ¼Æ¬
		srcImg = imread("C:\\Users\\mrxu\\Desktop\\a.jpg");		
#else
		capL >> srcImg;
#endif
		vector<Rect> ret = fc.Detect(srcImg);

		for (int i = 0; i < ret.size(); i++)
		{
			cout << ret[i].x << ret[i].y << ret[i].x + ret[i].width << ret[i].y + ret[i].height << endl;
		}
		waitKey(1);
	}
	return 0;
}
#endif
