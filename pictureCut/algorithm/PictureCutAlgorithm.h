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

	double threshold1 ; // canny threshold 1
	double threshold2 ; // canny threshold 2
public:
	PictureCutAlgorithm( );
	PictureCutAlgorithm( String  fileName );
	PictureCutAlgorithm( cv::Mat mat );
	~PictureCutAlgorithm(void);
	// variable 所有的轮廓的描述 vector

	void setThreshold( double  threshold1 , double  threshold2 );

	//
	void colorPictureToGray();
	//检测出所有轮廓 // 
	//直接读取文件
	void pickOutAllContours( );
	void pickOutAllContoursInMat( cv::Mat in ,vector<vector<Point> > *out );
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
	vector<vector<Point> > confirmContour( vector<Point>  *needConfirmContour );
	vector<vector<Point> > confirmContours( vector<vector<Point> > needConfirmContours  );
};

