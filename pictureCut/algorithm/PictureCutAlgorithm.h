#pragma once
#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"   
using namespace cv;
using namespace std;

class PictureCutAlgorithm
{
private:
	String  fileName;
	cv::Mat srcMat;
	cv::Mat canny_output;
	//the pram. for findContours,  
	vector<vector<Point> > contours;
public:
	PictureCutAlgorithm( String  fileName );
	~PictureCutAlgorithm(void);
	// variable 所有的轮廓的描述 vector


	//
	void colorPictureToGray();
	//检测出所有轮廓 // 
	//直接读取文件
	void pickOutAllContours();
	void showAllContours();

	void allTest();


private:
	bool openFile();

};

