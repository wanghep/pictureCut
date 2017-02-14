#include "StdAfx.h"
#include "PictureCutAlgorithm.h"


using namespace cv;
using namespace std;
void coordinateTranslate( vector<vector<Point> >*contours , CvRect  transRect );

PictureCutAlgorithm::PictureCutAlgorithm( )
{
	this->srcMat = NULL;
	threshold1 = 80;
	threshold2 = 255;
}

PictureCutAlgorithm::PictureCutAlgorithm( String  fileName )
{
	this->fileName = fileName;
	threshold1 = 80;
	threshold2 = 255;
}

PictureCutAlgorithm::PictureCutAlgorithm( cv::Mat mat )
{
	this->srcMat = mat;
	threshold1 = 80;
	threshold2 = 255;
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

void PictureCutAlgorithm::setThreshold( double  threshold1 , double  threshold2 )
{
	this->threshold1 = threshold1;
	this->threshold2 = threshold2;
}

//need to call cvReleaseMat( ) after use return mat finished;
cv::Mat* changeRGBToStandGray(cv::Mat * src, Vec3b &elem )
{

	cv::Mat *img = new cv::Mat( src->rows, src->cols, CV_8UC1 ); 
	//cvGetImage(src, img);
	for (int i = 0; i < img->rows; i++)
	{
		for (int j = 0; j < img->cols; j++)
		{
			//img->at<uchar>(i,j);		
			Vec3b &s = src->at<Vec3b>(i,j);//s.val[0], s.val[1], s.val[2] B G R
			int tempB = abs(s[0] - elem[0]);
			//s.val[0] = tempB;

			int tempG = abs(s[1] - elem[1]);
			//s.val[1] = tempG;

			int tempR = abs(s[2] - elem[2]);
			//s.val[2] = tempR;

			//255*255*3 相当于 255
			img->at<uchar>(i,j) = (tempB*tempB + tempG*tempG + tempR*tempR)/765;
		}

		
	}

	return img;
}


// 此 contour 肯定在当前的mat中 , 返回新的contours
vector<vector<Point> > PictureCutAlgorithm::confirmContour( vector<Point>  *needConfirmContour )
{
	map< int , vector<Point>*> PointSet ;
	

	Rect roiRect( 0 , 0 , 1 , 1 );

	int Left,Right,Top,Bottom ;
	Left = Right = (*needConfirmContour)[0].x;
	Top = Bottom = (*needConfirmContour)[0].y;

	for( int i = 0 ; i < needConfirmContour->size() ; i++ )
	{
		Point cur = (*needConfirmContour)[i];

		if( Left > cur.x )
		{
			Left = cur.x;
		}
		if( Right < cur.x )
		{
			Right = cur.x;
		}
		if( Top > cur.y )
		{
			Top = cur.y;
		}
		if( Bottom < cur.y )
		{
			Bottom = cur.y;
		}

		Vec3b &elem = srcMat.at<Vec3b>(cur);
		//Vec3b &deleteMatEle = deleteMat.at<Vec3b>(cur);

		//CvScalar value = cvGet2D( &srcMat , cur.y , cur.x );

		int total = elem[0] +  elem[1] +  elem[2];
		if( PointSet.count( total )<=0 ) 
		{
			vector<Point> *allocVector = new vector<Point>;

			PointSet.insert( make_pair(total , allocVector ));
		}
		(*PointSet[total]).push_back( cur );
	}

	//cv::Mat deleteMat = cv::Mat::zeros( srcMat.size() ,CV_8UC3 );
	//imshow( "show2", deleteMat );
	//waitKey( 9999999999 );

	int NumberOfMaxPointSet = 0;
	Point selectPoint;

	map< int , vector<Point>*>  :: iterator   iter;
	for(  iter=PointSet.begin();     iter!=PointSet.end();   iter++)
    {
        vector<Point> *p = iter ->second;
		if( p->size() > NumberOfMaxPointSet )
		{
			NumberOfMaxPointSet = p->size();
			selectPoint = (*p)[0];
		}
		delete( p );
    }

	Left = ( Left > 10 ) ? ( Left -10 ): 0 ;
	Top = ( Top > 10 ) ? ( Top -10 ): 0 ;
	Right = ( Right + 10 ) < srcMat.cols  ? ( Right + 10 ): ( srcMat.cols -2 ) ;
	Bottom = ( Bottom + 10 ) < srcMat.rows  ? ( Bottom + 10 ): ( srcMat.rows -2 ) ;

	roiRect.x = Left;
	roiRect.y = Top;
	roiRect.width = Right - Left + 1;
	roiRect.height = Bottom - Top + 1;

	static int enterCount = 0;
	enterCount++;
	
	cv::Mat roiMat = srcMat( roiRect ).clone();
	
	
	Vec3b &standardS = srcMat.at<Vec3b>( selectPoint ); 
	if( enterCount == 65 )
	{
		enterCount = 65;
	}
	cv::Mat * grayMat = changeRGBToStandGray( &roiMat, standardS );

	if( enterCount == 65 )
	{	
		for( int i = -5 ; i < 10 ; i ++ )
		{
			for( int j = -5 ; j < 10 ; j ++ )
			{
				roiMat.at<Vec3b>( selectPoint.x - Left + i, selectPoint.y - Top + j )[0] = 0;
				roiMat.at<Vec3b>( selectPoint.x - Left + i, selectPoint.y - Top + j )[1] = 0;
				roiMat.at<Vec3b>( selectPoint.x - Left + i, selectPoint.y - Top + j )[2] = 255;
			}
		}
		

		imshow("roiMat", roiMat );
		imshow("grayMat", *grayMat );
		waitKey();
	}

	vector<vector<Point> > NewContours;
	pickOutAllContoursInMat( (*grayMat) , &NewContours );

	//contours 坐标转换
	coordinateTranslate( &NewContours , roiRect );

	delete grayMat;

	return NewContours;


}

vector<vector<Point> > PictureCutAlgorithm::confirmContours( vector<vector<Point> > needConfirmContours  )
{
	vector<vector<Point> > ret;
	for( int i = 0 ; i < needConfirmContours.size() ; i++ )
	{
		vector<vector<Point> > result = confirmContour( &needConfirmContours[i] );

		ret.insert( ret.end(), result.begin() , result.end() );
	}


	return ret;
}

void PictureCutAlgorithm::pickOutAllContoursInMat( cv::Mat in ,vector<vector<Point> > *out )
{
	//blur(srcMat, srcMat, Size(3, 3));

	vector<Vec4i> hierarchy;

	cv::Mat canny_output;
	/// Detect edges using canny  
	//imshow( "srcMat" , srcMat );
	Canny(in, canny_output, threshold1, threshold2, 3);

	//imshow( "canny_output" , canny_output );
	/// Find contours  
	
	(*out).empty();	// first empty
	findContours(canny_output, (*out), hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));
	//CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE  

	double maxarea = 0;
	int maxAreaIdx = 0;

	for (int i = 0; i<(*out).size(); i++)
	{

		double tmparea = fabs(contourArea((*out)[i]));
		if (tmparea>maxarea)
		{
			maxarea = tmparea;
			maxAreaIdx = i;
			continue;
		}

		if (tmparea < 10)
		{
			//删除面积小于设定值的轮廓  
			(*out).erase((*out).begin() + i);
			std::wcout << "delete a small area" << std::endl;
			continue;
		}
		//计算轮廓的直径宽高  
		Rect aRect = boundingRect((*out)[i]);
		//if ((aRect.width / aRect.height)<whRatio)
		//{
			//删除宽高比例小于设定值的轮廓  
		//	contours.erase(contours.begin() + i);
		//	std::wcout << "delete a unnomalRatio area" << std::endl;
		//	continue;
		//}
	}
	return ;
}

	//检测出所有轮廓 // 
	//直接读取文件
void PictureCutAlgorithm::pickOutAllContours()
{
	vector<vector<Point> > allContours;

	pickOutAllContoursInMat( srcMat , &allContours );	

	//contours = confirmContours( allContours  );

	contours = allContours;
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

// 根据 srcRect 映射到 dstRect 的关系，计算出 point 应该映射到哪里（rePoint）
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


void coordinateTranslate( vector<vector<Point> >*contours , CvRect  transRect )
{
	int contoursSize = contours->size() ;

	for( int i = 0 ; i < contoursSize ; i++  )
	{
		for( int j = 0 ; j < (*contours)[i].size() ; j++ )
		{
			(*contours)[i][j].x += transRect.x;
			(*contours)[i][j].y += transRect.y;
		}
	}
}
