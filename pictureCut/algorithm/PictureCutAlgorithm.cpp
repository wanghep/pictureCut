#include "StdAfx.h"
#include "PictureCutAlgorithm.h"


using namespace cv;
using namespace std;


PictureCutAlgorithm::PictureCutAlgorithm( String  fileName )
{
	this->fileName = fileName;
}


PictureCutAlgorithm::~PictureCutAlgorithm(void)
{
}


bool PictureCutAlgorithm::openFile()
{
	cv::Mat srcMat;

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
	//������������ // 
	//ֱ�Ӷ�ȡ�ļ�
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
			//ɾ�����С���趨ֵ������  
			contours.erase(contours.begin() + i);
			std::wcout << "delete a small area" << std::endl;
			continue;
		}
		//����������ֱ�����  
		Rect aRect = boundingRect(contours[i]);
		//if ((aRect.width / aRect.height)<whRatio)
		//{
			//ɾ����߱���С���趨ֵ������  
		//	contours.erase(contours.begin() + i);
		//	std::wcout << "delete a unnomalRatio area" << std::endl;
		//	continue;
		//}
	}
}

void PictureCutAlgorithm::showAllContours()
{
	/// Draw contours,��ɫ����  
	vector<Vec4i> hierarchy;
	RNG rng(0xFFFFFFFF);
	cv::Mat dstMat;
	dstMat = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//�����ɫ  
		//CvScalar color = CV_RGB(255, 0, 0);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(dstMat, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	
}


void PictureCutAlgorithm::allTest()
{
	//IplImage * img = NULL;
	//img = cvLoadImage("image.jpg", 0);


	//Mat picture = imread("image.jpg",0);//ͼƬ������ӵ�����Ŀ¼��
	//Ҳ���Ǻ�help.cpp�ļ�����һ���ļ����£�����
	//imshow("���Գ���", picture);
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
			//ɾ�����С���趨ֵ������  
			contours.erase(contours.begin() + i);
			std::wcout << "delete a small area" << std::endl;
			continue;
		}
		//����������ֱ�����  
		Rect aRect = boundingRect(contours[i]);
		//if ((aRect.width / aRect.height)<whRatio)
		//{
			//ɾ����߱���С���趨ֵ������  
		//	contours.erase(contours.begin() + i);
		//	std::wcout << "delete a unnomalRatio area" << std::endl;
		//	continue;
		//}
	}
	/// Draw contours,��ɫ����  
	RNG rng(0xFFFFFFFF);
	dst = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//�����ɫ  
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
