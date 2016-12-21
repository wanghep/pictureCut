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
	// variable ���е����������� vector


	//
	void colorPictureToGray();
	//������������ // 
	//ֱ�Ӷ�ȡ�ļ�
	void pickOutAllContours();
	void showAllContours();

	void allTest();


private:
	bool openFile();

};

