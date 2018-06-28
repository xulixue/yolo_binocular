#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <time.h>
#include <vector>
#include <string>

#include "SelDepth.h"

using namespace std;
using namespace cv;

clock_t startTime, endTime;

vector<string> splitx(const string& str, const string& delim) {
    vector<string> res;
    if("" == str) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char * strs = new char[str.length() + 1] ; //不要忘了
    strcpy(strs, str.c_str());

    char * d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while(p) {
        string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(NULL, d);
    }

    return res;
}

int get_pos(vector<SelBox> &sbs){
    ifstream file;
    file.open("/home/mrxu/prj_data/det_rlt.txt");
    if(!file.is_open()){
        cout << " failed read det_rlt.txt" << endl;
        return 0;
    }
    std::string strLine;
    int lines_cnt = 0;
    while(getline(file,strLine))
    {
        if(lines_cnt ++ >= 1){
            if(strLine.empty())
                continue;
            cout<<strLine <<endl;
            std::vector<string> sv = splitx(strLine,"|");
            SelBox sb;
            sb.name = sv[0];
            std::vector<string> sv_pos = splitx(sv[1],",");
            int a[4];
            for(int i = 0; i < 4; i++)
                a[i] = atoi(sv_pos[i].c_str());
            sb.lft = a[0];
            sb.rit = a[1];
            sb.top = a[2];
            sb.botm = a[3];
            sbs.push_back(sb);
        }
    }
    file.close();
    return 1;
}


int main()
{

	SelDepth sdp;
//    VideoCapture capL(0);
//    VideoCapture capR(1);

	while (true)
	{
    #if 1
        Mat srcImage1, srcImage2, srcL1, srcR1;
    		srcL1 = imread("/home/mrxu/Desktop/left.jpg"/*, IMREAD_GRAYSCALE*/);
    		srcR1 = imread("/home/mrxu/Desktop/right.jpg"/*, IMREAD_GRAYSCALE*/);

        resize(srcL1, srcImage1, Size(640,480));
        resize(srcR1, srcImage2, Size(640,480));

    #else
    		Mat srcImage1, srcImage2;
    		capL >> srcImage1;
    		capR >> srcImage2;
    #endif

        imwrite("/home/mrxu/prj_data/left.jpg", srcImage1);

	if (srcImage1.empty() || srcImage2.empty())
		{
        cout << "load camer failed" << endl;;
        system("pause");
        return -1;
		}
		else
     //   cout << "ini camer ok!" << endl << endl;

      SelBox sl1 = { 20, 100, 300, 350, "class1" };
      SelBox sl2 = { 280, 360, 200, 280, "class2" };
      SelBox sl3 = { 100, 200, 300, 350, "class2" };

		startTime = clock();

		sdp.setPixOffset(0, 3);
		sdp.RunSURF(srcImage1, srcImage2);
		vector<SelBox> sbs;
//        sbs.push_back(sl1);
//		  sbs.push_back(sl2);
//        sbs.push_back(sl3);

        int r = get_pos(sbs);       // read pos from file.
        if(r == 0)
            return 0;


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
