#include "stdafx.h"
#include "SelDepth.h"

using namespace std;
using namespace cv;

SelDepth::SelDepth()
{
}
SelDepth::~SelDepth()
{
}

void SelDepth::RunSURF(Mat &srcL, Mat &srcR){
    Mat srcImage1, srcImage2;
    Mat srcL1, srcR1;




#if 0
    Sobel(srcL, srcL1, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(srcL1, srcImage1);

    Sobel(srcR, srcR1, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(srcR1, srcImage2);
    imshow("srcImage2", srcImage2);
#else
    srcImage1 = srcL;
    srcImage2 = srcR;
#endif

    int minHessian = 2000;

    SurfFeatureDetector detector(minHessian);
    detector.detect(srcImage1, keypoint1);
    detector.detect(srcImage2, keypoint2);

    SurfDescriptorExtractor descriptorExtractor;
    Mat descriptors1, descriptors2;

    descriptorExtractor.compute(srcImage1, keypoint1, descriptors1);
    descriptorExtractor.compute(srcImage2, keypoint2, descriptors2);

    BFMatcher matcher(NORM_L2);
    if (descriptors1.dims > 0 && descriptors2.dims > 0)
        matcher.match(descriptors1, descriptors2, matches);
    else
    {
        matches.clear();
        cout << "err size!" << endl;
    }



}

bool SelDepth::InsideROI(int x, int y, SelBox sb){
    return x >= sb.lft && x <= sb.rit && y >= sb.top && y <= sb.botm;
}

void SelDepth::SelMatchEach(SelBox &sb, bool showDebug){
    vector<DMatch> selMatch;
    vector<int> x2_x1;
    int sum_deep = 0;
    int sum_cnt = 0;
    for (int i = 0; i < matches.size(); i++)
    {
        DMatch tmp = matches[i];
        int x1 = keypoint1[tmp.queryIdx].pt.x;
        int y1 = keypoint1[tmp.queryIdx].pt.y;
        int x2 = keypoint2[tmp.trainIdx].pt.x;
        int y2 = keypoint2[tmp.trainIdx].pt.y;
        if (InsideROI(x1, y1, sb))
        {
            if (showDebug)
                cout << "x1:" << x1 << "  y1:" << y1 << "  x2:" << x2 << "  y2:" << y2 << "  x2-x1:" << x2 - x1 << "  y2-y1:" << y2 - y1;
            if (y2 - y1 >= OFFSET_VERTICAL_Y2_Y1 - OFFSET_VERTICAL_Y2_Y1_PERMIT && y2 - y1 <= OFFSET_VERTICAL_Y2_Y1 + OFFSET_VERTICAL_Y2_Y1_PERMIT)
            {
                sb.selMatch.push_back(tmp);
                if (x2 - x1 > 0)
                {
                    sum_deep += x2 - x1;
                    sum_cnt++;
                    if (showDebug)
                        cout << "  this is passed!";
                }
            }
            if (showDebug)
                cout << endl;
        }
    }
    if (sum_cnt > 0)
    {
        sum_deep /= sum_cnt;
        sb.ret_x2_de_x1 = sum_deep;
    }
    else
    {
        sum_deep = 0;
        sb.ret_x2_de_x1 = -1;
    }
    if (showDebug)
        cout << "sum_cnt:" << sum_cnt << "  aver: (x2-x1)= " << sum_deep << endl;
}

void SelDepth::SelMatch(vector<SelBox> &sbs){
    //	cout << "sbs.size():" << sbs.size() << endl;
    for (int i = 0; i < sbs.size(); i++)
    {
        SelMatchEach(sbs[i], false);
    }
    GetAllSelMatch(sbs);
}

void SelDepth::GetAllSelMatch(vector<SelBox> &sbs){
    for (int i = 0; i < sbs.size(); i++)
    {
        for (int j = 0; j < sbs[i].selMatch.size(); j++)
        {
            selAllMatches.push_back(sbs[i].selMatch[j]);
        }
    }
}

void SelDepth::ShowAllSelMatch(Mat &imgL, Mat &imgR){
    //if (selAllMatches.empty())
    //	GetAllSelMatch();
    Mat matchImage;
//	cout << "selAllMatches:" << selAllMatches.size() << endl;
    drawMatches(imgL, keypoint1, imgR, keypoint2, selAllMatches, matchImage);

    namedWindow("Match", WINDOW_AUTOSIZE);
    imshow("Match", matchImage);
    waitKey(10);
}

void SelDepth::GetAllDeepth(vector<SelBox> &sbs){
    for (int i = 0; i < sbs.size(); i++){
        if (sbs[i].ret_x2_de_x1 == -1){
            sbs[i].deepth_mm = -1;
        }
        else{
            sbs[i].deepth_mm = (int)(100.0 * 86 / sbs[i].ret_x2_de_x1);
        }
        cout << sbs[i].name << "deepth: " << sbs[i].deepth_mm << endl;
    }


}

void SelDepth::Clear(void){
    matches.clear();
    selAllMatches.clear();
    keypoint1.clear();
    keypoint2.clear();
}

