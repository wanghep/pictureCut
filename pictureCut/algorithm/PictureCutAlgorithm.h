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
	PictureCutAlgorithm( );
	PictureCutAlgorithm( String  fileName );
	PictureCutAlgorithm( cv::Mat mat );
	~PictureCutAlgorithm(void);
	// variable ���е����������� vector


	//
	void colorPictureToGray();
	//������������ // 
	//ֱ�Ӷ�ȡ�ļ�
	void pickOutAllContours();
	vector<vector<Point> > getAllContours();
	void showAllContours();

	void allTest();

	
	void SetMat( cv::Mat mat );

private:
	bool openFile();

	vector<vector<Point> > rectFind(Rect rect);
	void deleteContour(vector<Point> delVec);
	bool isVecEqual(vector<Point> value1, vector<Point> value2);
	vector<Point> clickFind(Point point);
};

