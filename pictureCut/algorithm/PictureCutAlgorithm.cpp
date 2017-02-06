#include "StdAfx.h"
#include "PictureCutAlgorithm.h"


using namespace cv;
using namespace std;

PictureCutAlgorithm::PictureCutAlgorithm( )
{
	this->srcMat = NULL;
}

PictureCutAlgorithm::PictureCutAlgorithm( String  fileName )
{
	this->fileName = fileName;
}

PictureCutAlgorithm::PictureCutAlgorithm( cv::Mat mat )
{
	this->srcMat = mat;
}


PictureCutAlgorithm::~PictureCutAlgorithm(void)
{
}

void PictureCutAlgorithm::SetMat( cv::Mat mat )
{
	this->srcMat = mat;
}

bool PictureCutAlgorithm::openFile()
{
	
	if( fileName.empty() )
	{
		std::cout << "read data error!" << std::endl;
		return false;
	}
	else
	{
		cv::Mat src, dst, canny_output;
		/// Load source image and convert it to gray  
		
		srcMat = imread(fileName, 0);
	}
}

void PictureCutAlgorithm::colorPictureToGray()
{
}
	//检测出所有轮廓 // 
	//直接读取文件
void PictureCutAlgorithm::pickOutAllContours()
{
	blur(srcMat, srcMat, Size(3, 3));


	
	vector<Vec4i> hierarchy;

	/// Detect edges using canny  
	Canny(srcMat, canny_output, 80, 255, 3);
	/// Find contours  
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE  

	double maxarea = 0;
	int maxAreaIdx = 0;

	for (int i = 0; i<contours.size(); i++)
	{

		double tmparea = fabs(contourArea(contours[i]));
		if (tmparea>maxarea)
		{
			maxarea = tmparea;
			maxAreaIdx = i;
			continue;
		}

		if (tmparea < 50)
		{
			//删除面积小于设定值的轮廓  
			contours.erase(contours.begin() + i);
			std::wcout << "delete a small area" << std::endl;
			continue;
		}
		//计算轮廓的直径宽高  
		Rect aRect = boundingRect(contours[i]);
		//if ((aRect.width / aRect.height)<whRatio)
		//{
			//删除宽高比例小于设定值的轮廓  
		//	contours.erase(contours.begin() + i);
		//	std::wcout << "delete a unnomalRatio area" << std::endl;
		//	continue;
		//}
	}
}

vector<vector<Point> > PictureCutAlgorithm::getAllContours()
{
	return contours;
}

void PictureCutAlgorithm::showAllContours()
{
	/// Draw contours,彩色轮廓  
	vector<Vec4i> hierarchy;
	RNG rng(0xFFFFFFFF);
	cv::Mat dstMat;
	dstMat = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//随机颜色  
		//CvScalar color = CV_RGB(255, 0, 0);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(dstMat, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	
	imshow("测试程序", dstMat);
	waitKey(20150901);
}


void PictureCutAlgorithm::allTest()
{
	//IplImage * img = NULL;
	//img = cvLoadImage("image.jpg", 0);


	//Mat picture = imread("image.jpg",0);//图片必须添加到工程目录下
	//也就是和help.cpp文件放在一个文件夹下！！！
	//imshow("测试程序", picture);
	//waitKey(20150901);


	cv::Mat src, dst, canny_output;
	/// Load source image and convert it to gray  
	src = imread("image.jpg", 0);

	if (!src.data)
	{
		std::cout << "read data error!" << std::endl;
	}
	blur(src, src, Size(3, 3));


	//the pram. for findContours,  
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny  
	Canny(src, canny_output, 80, 255, 3);
	/// Find contours  
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE  

	double maxarea = 0;
	int maxAreaIdx = 0;

	for (int i = 0; i<contours.size(); i++)
	{

		double tmparea = fabs(contourArea(contours[i]));
		if (tmparea>maxarea)
		{
			maxarea = tmparea;
			maxAreaIdx = i;
			continue;
		}

		if (tmparea < 50)
		{
			//删除面积小于设定值的轮廓  
			contours.erase(contours.begin() + i);
			std::wcout << "delete a small area" << std::endl;
			continue;
		}
		//计算轮廓的直径宽高  
		Rect aRect = boundingRect(contours[i]);
		//if ((aRect.width / aRect.height)<whRatio)
		//{
			//删除宽高比例小于设定值的轮廓  
		//	contours.erase(contours.begin() + i);
		//	std::wcout << "delete a unnomalRatio area" << std::endl;
		//	continue;
		//}
	}
	/// Draw contours,彩色轮廓  
	RNG rng(0xFFFFFFFF);
	dst = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//随机颜色  
		//CvScalar color = CV_RGB(255, 0, 0);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(dst, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	// Create Window  
	char* source_window = "countors";
	namedWindow(source_window, CV_WINDOW_NORMAL);
	imshow(source_window, dst);
	waitKey(0);
}




//矩形里面找 contours
vector<vector<Point> > PictureCutAlgorithm::rectFind(Rect rect)
{
	vector<vector<Point> > reVec;
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> tempVec = contours[i];
		for (int j = 0; j < tempVec.size(); j++)
		{
			if (rect.x < tempVec[j].x && tempVec[j].x < rect.x + rect.width && rect.y < tempVec[j].y && tempVec[j].y < rect.y + rect.height)
			{
				reVec.push_back(tempVec);
				break;
			}
		}

	}
	return reVec;
}


//删除指定轮廓
void PictureCutAlgorithm::deleteContour(vector<Point> delVec)
{
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> temp = contours[i];
		if (isVecEqual(delVec, temp))
		{
			contours.erase(contours.begin() + i);
		}
	}
}

//判断两个轮廓是否相等
bool PictureCutAlgorithm::isVecEqual(vector<Point> value1, vector<Point> value2)
{
	if (value1.size() == value2.size())
	{
		for (int i = 0; i < value1.size(); i++)
		{
			if (value1[i] != value2[i])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}


//根据点找到最近的轮廓（ ）
vector<Point> PictureCutAlgorithm::clickFind(Point point)
{
	vector<Point> reVec;
	int isInitValue = 1;
	int minInstance;
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> tempVec = contours[i];
		for (int j = 0; j < tempVec.size(); j++)
		{
			Point tempPoint = tempVec[j];
			if (isInitValue)
			{
				minInstance = (tempPoint.x - point.x)*(tempPoint.x - point.x) + (tempPoint.y - point.y)*(tempPoint.y - point.y);
				reVec = tempVec;
				isInitValue = 0;
			}else
			{
				int temp = (tempPoint.x - point.x)*(tempPoint.x - point.x) + (tempPoint.y - point.y)*(tempPoint.y - point.y);
				if (temp < minInstance)
				{
					minInstance = temp;
					reVec = tempVec;
				}
			}
		}

	}
	return reVec;
}

void refect( Rect srcRect, Rect dstRect, Point point, Point *rePoint){
	int img_height = srcRect.height;
	int img_width = srcRect.width;
	vector<Point2f> corners(4);
	corners[0] = Point2f( srcRect.x, srcRect.y );
	corners[1] = Point2f( srcRect.x + srcRect.width , srcRect.y );
	corners[2] = Point2f( srcRect.x , srcRect.y + srcRect.height );
	corners[3] = Point2f( srcRect.x + srcRect.width, srcRect.y + srcRect.height );
	vector<Point2f> corners_trans(4);

	corners_trans[0] = Point2f( dstRect.x, dstRect.y );
	corners_trans[1] = Point2f( dstRect.x + dstRect.width , dstRect.y );
	corners_trans[2] = Point2f( dstRect.x , dstRect.y + dstRect.height );
	corners_trans[3] = Point2f( dstRect.x + dstRect.width, dstRect.y + dstRect.height );
  
    Mat transform ;
	transform = getPerspectiveTransform(corners,corners_trans);
	vector<Point2f> ponits, points_trans;
    ponits.push_back(point);

    perspectiveTransform( ponits, points_trans, transform);
	rePoint->x = points_trans[0].x;
	rePoint->y = points_trans[0].y;
}
