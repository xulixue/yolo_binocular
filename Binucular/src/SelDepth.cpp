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

    //cvtColor(srcL, srcL1, CV_BGR2GRAY);
    //cvtColor(srcR, srcR1, CV_BGR2GRAY);
    ////��ʹ��3*3�ں�������
    //blur(srcL1, srcImage1, Size(3, 3));
    ////����canny����
    //Canny(srcImage1, srcImage1, 3, 9, 3);
    ////��ʹ��3*3�ں�������
    //blur(srcR1, srcImage2, Size(3, 3));
    ////����canny����
    //Canny(srcImage2, srcImage2, 3, 9, 3);
    //imshow("canny out ", srcImage1);


#if 0
    //��y�����ݶ�
    Sobel(srcL, srcL1, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(srcL1, srcImage1);

    //��y�����ݶ�
    Sobel(srcR, srcR1, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
    convertScaleAbs(srcR1, srcImage2);
    imshow("y��soble", srcImage2);
#else
    srcImage1 = srcL;
    srcImage2 = srcR;
#endif
    //��������ͼ���е�������
    int minHessian = 2000;      //����Hessian������ֵ

    SurfFeatureDetector detector(minHessian);       //����Surf������
    detector.detect(srcImage1, keypoint1);
    detector.detect(srcImage2, keypoint2);

    //��������������������
    SurfDescriptorExtractor descriptorExtractor;
    Mat descriptors1, descriptors2;

    descriptorExtractor.compute(srcImage1, keypoint1, descriptors1);
    descriptorExtractor.compute(srcImage2, keypoint2, descriptors2);

    //ʹ��BruteForceMatcher����������ƥ��
    BFMatcher matcher(NORM_L2);
    if (descriptors1.dims > 0 && descriptors2.dims > 0)
        matcher.match(descriptors1, descriptors2, matches);
    else
    {
        matches.clear();
        cout << "err size!" << endl;
    }


    /* ������knn�ķ�������ɸѡ�ģ��ҿ����ò��ϡ������Լ���ɸѡ��
    BFMatcher matcher(NORM_L2, false);//����һ��ƥ������
    vector<vector<DMatch>> matches2;//����һ����������װ�����ڵ��ʹν��ڵ�
    vector<DMatch>matches;//����һ����������װ���������ĵ�
    matcher.match(descriptors1, descriptors2, matches);//����ƥ��
    const float ratio = 0.7;//����ֵ��Ϊ0.7  �����Լ�����
    matches.clear();//����matches
    matcher.knnMatch(descriptors1, descriptors2, matches2, 2);//����knnmatch
    for (int n = 0; n < matches2.size(); n++)
    {
    DMatch& bestmatch = matches2[n][0];
    DMatch& bettermatch = matches2[n][1];
    if (bestmatch.distance < ratio*bettermatch.distance)//ɸѡ�����������ĵ�
    {
    matches.push_back(bestmatch);//�����������ĵ㱣����matches
    }
    }
    cout << "match����:" << matches.size() << endl;
    */

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
                sb.selMatch.push_back(tmp);			//��һ��ɸѡ��
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
    if (sum_cnt > 0)				//����0
    {
        sum_deep /= sum_cnt;
        sb.ret_x2_de_x1 = sum_deep;
    }
    else
    {
        sum_deep = 0;
        sb.ret_x2_de_x1 = -1;  // ɸѡ֮��û��ƥ������Ҫ
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
    //����ƥ��������
    Mat matchImage;
//	cout << "selAllMatches:" << selAllMatches.size() << endl;
    drawMatches(imgL, keypoint1, imgR, keypoint2, selAllMatches, matchImage);

    //��ʾƥ����ͼ��
    namedWindow("Match", WINDOW_AUTOSIZE);
    imshow("Match", matchImage);
    waitKey(10);
}

void SelDepth::GetAllDeepth(vector<SelBox> &sbs){
    for (int i = 0; i < sbs.size(); i++){
        if (sbs[i].ret_x2_de_x1 == -1){				//û���ҵ�ƥ�䣬û�в⵽������ֱ�ӷ���-1
            sbs[i].deepth_mm = -1;
        }
        else{
            sbs[i].deepth_mm = (int)(100.0 * 86 / sbs[i].ret_x2_de_x1);
        }
        cout << "deepth: " << sbs[i].deepth_mm << endl;
    }


}

void SelDepth::Clear(void){
    matches.clear();			//����Ҫ�����ڴ档
    selAllMatches.clear();
    keypoint1.clear();
    keypoint2.clear();
}

