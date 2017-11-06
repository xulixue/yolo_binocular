#include "stdafx.h"

#if 1

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


int main()
{
    FaceCascade fc("//home//mrxu//Desktop//haarcascade_frontalface_alt2.xml");
    cout << fc.cascade.empty() << endl;
	SelDepth sdp;
    VideoCapture capL(2);
    VideoCapture capR(1);

	while (true)
	{
#if 0
		Mat srcImage1 = imread("C:\\Users\\mrxu\\Desktop\\left.jpg", IMREAD_GRAYSCALE);
		Mat srcImage2 = imread("C:\\Users\\mrxu\\Desktop\\right.jpg", IMREAD_GRAYSCALE);
#else
		Mat srcImage1, srcImage2;
		capL >> srcImage1;
		//srcImage2 = srcImage1;
		capR >> srcImage2;

		//imwrite("C:\\Users\\mrxu\\Desktop\\left.jpg", srcImage1);
		//imwrite("C:\\Users\\mrxu\\Desktop\\right.jpg", srcImage2);
#endif

		if (srcImage1.empty() || srcImage2.empty())
		{

            cout << "load camer failed";
			system("pause");
			return -1;
		}
		else
            cout << "ini camer ok!" << endl << endl;

        SelBox sl1 = { 20, 100, 300, 350, "class1" };
        SelBox sl2 = { 280, 360, 200, 280, "class2" };
        SelBox sl3 = { 100, 200, 300, 350, "class2" };

		startTime = clock();
		//sdp.selbox.push_back(sl1);
		//sdp.selbox.push_back(sl2);
		//sdp.SelMatchEach(sl1, true);
		//sdp.SelMatchEach(sl2, true);
		sdp.RunSURF(srcImage1, srcImage2);

		vector<SelBox> sbs;
//        sbs.push_back(sl1);
//		  sbs.push_back(sl2);
//        sbs.push_back(sl3);

		vector<Rect> ret = fc.Detect(srcImage1);

		for (int i = 0; i < ret.size(); i++){
			//cout << ret[i].x << ret[i].y << ret[i].x + ret[i].width << ret[i].y + ret[i].height << endl;
			SelBox sb = { ret[i].x, ret[i].x + ret[i].width, ret[i].y, ret[i].y + ret[i].height, "class1" };
			sbs.push_back(sb);
		}

		sdp.SelMatch(sbs);
		sdp.GetAllDeepth(sbs);



		endTime = clock();
		cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

		sdp.ShowAllSelMatch(srcImage1, srcImage2);
		sdp.Clear();
	}


	waitKey(0);
//	getchar();
	return 0;
}
#endif
