
// pictureCutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pictureCut.h"
#include "pictureCutDlg.h"
#include "afxdialogex.h"
#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"   
using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>

#include "FileStream.hpp"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
void coordinateTranslate( vector<vector<Point> >*contours , CvRect  transRect );
void refect( Rect srcRect, Rect dstRect, Point point, Point *rePoint);
bool contoursInRoi( vector<Point> contour , CvRect roiRect );

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpictureCutDlg 对话框




CpictureCutDlg::CpictureCutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpictureCutDlg::IDD, pParent)
{
	//showImageMat = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	cannyThreshold1 = 80;
	cannyThreshold2 = 255;
	showBuffer = NULL;
	bitBuffer = NULL;
	WorkMode = MODE_NULL;
}

CpictureCutDlg::~CpictureCutDlg()
{
	if( showBuffer != NULL )
	{
		delete []showBuffer;
	}
	if( bitBuffer != NULL )
	{
		delete []bitBuffer;
	}
	
}
void CpictureCutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_PICTURE, m_showPicture);
	DDX_Text(pDX, IDC_THREADSHOLD1, cannyThreshold1);
	DDV_MinMaxUInt(pDX, cannyThreshold1, 1, 10000);
	DDX_Text(pDX, IDC_THREADSHOLD2, cannyThreshold2);
	DDV_MinMaxUInt(pDX, cannyThreshold2, 1, 10000);
	DDX_Control(pDX, IDC_CLIP_IMAGE, m_selectbtn);
}

BEGIN_MESSAGE_MAP(CpictureCutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START2, &CpictureCutDlg::OnBnClickedStart2)
	ON_BN_CLICKED(IDC_CON_PEN, &CpictureCutDlg::OnBnClickedConPen)
	ON_BN_CLICKED(IDC_CON_RUBBER, &CpictureCutDlg::OnBnClickedRubble)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SHOW_PICTURE, &CpictureCutDlg::OnThemechangedShowPicture )
	ON_STN_CLICKED(IDC_SHOW_PICTURE, &CpictureCutDlg::OnClickedShowPicture)
	ON_BN_CLICKED(IDC_ZOOM_DOWN2, &CpictureCutDlg::OnBnClickedCut)
	ON_BN_CLICKED(IDC_ZOOM_DOWN3, &CpictureCutDlg::OnBnShowSourcePicture)
	ON_EN_CHANGE(IDC_EDIT1, &CpictureCutDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD1_UP, &CpictureCutDlg::OnBnClickedCannyThreshold1Up)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD1_DOWN, &CpictureCutDlg::OnBnClickedCannyThreshold1Down)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD2_UP, &CpictureCutDlg::OnBnClickedCannyThreshold2Up)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD2_DOWN, &CpictureCutDlg::OnBnClickedCannyThreshold2Down)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD1_UP5, &CpictureCutDlg::OnBnClickedCannyThreshold1Up5)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD1_DOWN5, &CpictureCutDlg::OnBnClickedCannyThreshold1Down5)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD2_UP5, &CpictureCutDlg::OnBnClickedCannyThreshold2Up5)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD2_DOWN5, &CpictureCutDlg::OnBnClickedCannyThreshold2Down5)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD1_UP10, &CpictureCutDlg::OnBnClickedCannyThreshold1Up10)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD1_DOWN10, &CpictureCutDlg::OnBnClickedCannyThreshold1Down10)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD2_UP10, &CpictureCutDlg::OnBnClickedCannyThreshold2Up10)
	ON_BN_CLICKED(IDC_CANNY_THRESHHOLD2_DOWN10, &CpictureCutDlg::OnBnClickedCannyThreshold2Down10)
	ON_EN_CHANGE(IDC_THREADSHOLD1, &CpictureCutDlg::OnEnChangeThreadshold1)
	ON_EN_CHANGE(IDC_THREADSHOLD2, &CpictureCutDlg::OnEnChangeThreadshold2)
	ON_BN_CLICKED(IDC_SHOW_CLICK_OUT_LIE, &CpictureCutDlg::OnBnClickedOutline)
	ON_BN_CLICKED(IDC_SAVE_CONTOURS, &CpictureCutDlg::OnBnClickedSaveContours)
	ON_BN_CLICKED(IDC_READ_CONTOURS2, &CpictureCutDlg::OnBnClickedReadContours2)
	ON_BN_CLICKED(IDC_CLIP_IMAGE, &CpictureCutDlg::OnBnCutOut)
	ON_STN_DBLCLK(IDC_SHOW_PICTURE, &CpictureCutDlg::OnDblclkShowPicture)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BIAODING, &CpictureCutDlg::OnBnClickedBiaoding)
	ON_BN_CLICKED(IDC_SAVE_CAL_RESULT, &CpictureCutDlg::OnBnClickedSaveCalResult)
	ON_BN_CLICKED(IDC_SHOW_BIAODING_RESULT, &CpictureCutDlg::OnBnClickedShowBiaodingResult)
	ON_BN_CLICKED(IDC_MERGE_AREA, &CpictureCutDlg::OnBnClickedMergeArea)
	ON_BN_CLICKED(CONTOUR_MEGER, &CpictureCutDlg::OnBnClickedMeger)
	ON_BN_CLICKED(CONTOUR_FIX, &CpictureCutDlg::OnBnClickedFix)
	ON_BN_CLICKED(IDC_SAVE_LAYOUT_FILE, &CpictureCutDlg::OnBnClickedSaveLayoutFile)
END_MESSAGE_MAP()


// CpictureCutDlg 消息处理程序

BOOL CpictureCutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//hBmp = AfxGetApp()->LoadIcon( IDR_MAINFRAME );

	m_selectbtn.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_SELECT)));   
	
	
	//m_selectbtn.SetBitmap(hBmp);

	selectList.empty();

	showSourcePicture = TRUE;
	dupAndZeorInitFlag = false;
	showOutline = FALSE;
	showClipImageFlag = true;
	calibrationFlag = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CpictureCutDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpictureCutDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		if( !showImageMat.empty() )
		{
			showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat , true );
		}
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CpictureCutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void OnMouseAction(int event,int x,int y,int flags,void *ustc);  //鼠标回调事件函数  
void CpictureCutDlg::OnBnClickedStart2()
{
	CFileDialog dlg(TRUE,("png"),(".png"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,("PNG file(*.png)|*.png|jpg file(*.jpg) |*.jpg|jpeg file(*.jpeg) |*.jpeg||"));

	dlg.DoModal();	

	if( dlg.GetPathName().IsEmpty() )
	{
		std::cout << "read data error!" << std::endl;
		return ;
	}
	else
	{
		/// Load source image and convert it to gray  
		
		cv::String cvFileName ;
		cvFileName = dlg.GetPathName().GetBuffer(0);
		srcMat = imread(cvFileName);

		//试着读取一下预处理的图片，如果没有预处理的图片还是要用原图片
		cv::Mat preHandleMat = imread(cvFileName+".prehandle.png");
		roiRect.x = 0;
		roiRect.y = 0;
		roiRect.height = srcMat.rows;
		roiRect.width = srcMat.cols;

		{
			dupAndZeorInitFlag = true;
			//cvtColor( srcMat , duplicateSrcMat,CV_BGR2BGRA );
			if(preHandleMat.rows != 0 )
			{//读取预处理文件成功
				duplicateSrcMat = preHandleMat.clone();
			}
			else
			{
				duplicateSrcMat = srcMat.clone();
			
			}

			backgroundMat = Mat::zeros( srcMat.size(), CV_8UC3); // 初始化背景色为黑色

			alphaMat = Mat( srcMat.size(),CV_8UC3,Scalar(1,1,1)); // 初始化 alpha矩阵
		}

		
		Update();
		/*
		namedWindow("view", WINDOW_AUTOSIZE);
		HWND hWnd = (HWND) cvGetWindowHandle("view");
		HWND hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem( IDC_SHOW_PICTURE )->m_hWnd);
		setMouseCallback("view",OnMouseAction); 
		::ShowWindow(hParent, SW_HIDE);
		imshow("view", showImageMat);
		*/

	}

	Pca.SetMat( srcMat );
	
}

void saveImage(byte * pTempData, int w, int h) {
	char imageName[100];
	FILE* fpFile = NULL;
	sprintf(imageName, "c://work//delete//image.raw");
	fpFile = fopen(imageName, "wt");
	if (!fpFile) {
		return;
	}
	fwrite(pTempData, sizeof(byte), w * h , fpFile);
	fclose(fpFile);
}

//内存中的图像数据拷贝到屏幕上 

void CpictureCutDlg::showMatImgToWnd( CWnd* pWnd, cv::Mat img , bool forceUpdate )
//void BitMatToWnd(CWnd* pWnd, cv::Mat img, CRect *Roi)
{

#if 1

	if(img.empty()) 
		return;
	
	CClientDC dc(pWnd); 
	hDC =dc.GetSafeHdc();
	if( forceUpdate )
	{
		pWnd->GetClientRect(drect); //(drect); (&drect); 两种方式均可，竟然





		int ichannels =img.channels(); 
		int depth = img.depth();
		if( bitBuffer == NULL )
		{
			bool continous = img.isContinuous();
			if( ichannels == 1) 
			{ 
				bitBuffer = new BYTE[40+4*256]; 
			} 
			else if( ichannels == 3) 
			{ 
				bitBuffer = new BYTE[sizeof(BITMAPINFO)]; 
			} 
			else if( ichannels == 4) 
			{ 
				bitBuffer = new BYTE[sizeof(BITMAPINFO)]; 
			} 
			else 
			{ 
				return; 
			}
		}


		if(bitBuffer == NULL) 
		{ 
			return; 
		}


		bitMapinfo = (BITMAPINFO *)bitBuffer; 
		bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
		bitMapinfo->bmiHeader.biHeight = -img.rows; //如果高度为正的，位图的起始位置在左下角。如果高度为负，起始位置在左上角。 
		bitMapinfo->bmiHeader.biWidth = img.cols; 
		bitMapinfo->bmiHeader.biPlanes = 1; // 目标设备的级别，必须为1 
		bitMapinfo->bmiHeader.biBitCount = ichannels *8; // 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一 
		bitMapinfo->bmiHeader.biCompression = BI_RGB; //位图压缩类型，必须是 0(不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一 
		bitMapinfo->bmiHeader.biSizeImage = 0; // 位图的大小，以字节为单位 
		bitMapinfo->bmiHeader.biXPelsPerMeter = 0; // 位图水平分辨率，每米像素数 
		bitMapinfo->bmiHeader.biYPelsPerMeter = 0; // 位图垂直分辨率，每米像素数 
		bitMapinfo->bmiHeader.biClrUsed = 0; // 位图实际使用的颜色表中的颜色数 
		bitMapinfo->bmiHeader.biClrImportant = 0; // 位图显示过程中重要的颜色数

		if(ichannels == 1) 
		{ 
			for(int i=0; i<256; i++) 
			{ //颜色的取值范围 (0-255) 
				bitMapinfo->bmiColors[i].rgbBlue =bitMapinfo->bmiColors[i].rgbGreen =bitMapinfo->bmiColors[i].rgbRed =(BYTE) i; 
			}

			bitMapinfo->bmiHeader.biClrUsed = 256; // 位图实际使用的颜色表中的颜色数 
		} 
		SetStretchBltMode(hDC, COLORONCOLOR);

		if( showBuffer == NULL )
		{
			showBuffer = new byte[ img.cols * img.rows * ichannels ]; 
		}
		unsigned char * move = img.data;
		for( int j = 0 ; j <  img.rows ; j++ )	
		{
			memcpy( showBuffer + j * img.cols * ichannels ,  move ,  img.cols * ichannels );
			move += img.step;
		
		}
		//saveImage( showBuffer , img.cols * ichannels , img.rows);
	
		imgWidth = img.cols;
		imgHeight = img.rows; //图像高度 
	}
	StretchDIBits(	hDC, 
					0, 
					0, 
					drect.right, //显示窗口宽度 
					drect.bottom, //显示窗口高度 
					0, 
					0, 
					imgWidth, //图像宽度 
					imgHeight, //图像高度 
					showBuffer, 
					bitMapinfo, 
					DIB_RGB_COLORS, 
					SRCCOPY 
	);

	//delete []showBuffer;
	//delete []bitBuffer;

#endif

}



//轮廓笔，用于在轮廓上添加点
void CpictureCutDlg::OnBnClickedConPen()
{
	
	WorkMode = MODE_PEN;
	 //一、设置此窗口中特定控件的光标  
  
 
}

//轮廓橡皮，用于删除轮廓上的点
void CpictureCutDlg::OnBnClickedRubble()
{
	WorkMode = MODE_RUBBER;
}


void CpictureCutDlg::OnThemechangedShowPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void  drawDashRect(cv::Mat& img,int linelength,int dashlength,CvRect* blob,CvScalar color,int thickness)  
{  
    int w=cvRound(blob->width);//width  
    int h=cvRound(blob->height);//height  
  
  
    int tl_x=cvRound(blob->x);//top left x  
    int tl_y=cvRound(blob->y);//top  left y  
  
  
    int totallength=dashlength+linelength;  
    int nCountX=w/totallength;//  
    int nCountY=h/totallength;//  
  
  
    CvPoint start,end;//start and end point of each dash  
  
  
    //draw the horizontal lines  
    start.y=tl_y;  
    start.x=tl_x;  
  
  
    end.x=tl_x;  
    end.y=tl_y;  
  
  
    for (int i=0;i<nCountX;i++)  
    {  
		end.x=tl_x+(i+1)*totallength-dashlength;//draw top dash line  
		end.y=tl_y;  
		start.x=tl_x+i*totallength;  
		start.y=tl_y;  
		line( img ,start,end,color,thickness);     
    }  
	for (int i=0;i<nCountX;i++)  
	{    
		start.x=tl_x+i*totallength;  
		start.y=tl_y+h;  
		end.x=tl_x+(i+1)*totallength-dashlength;//draw bottom dash line  
		end.y=tl_y+h;  
		line(img,start,end,color,thickness);       
	}  
  
  
	for (int i=0;i<nCountY;i++)  
	{    
		start.x=tl_x;  
		start.y=tl_y+i*totallength;  
		end.y=tl_y+(i+1)*totallength-dashlength;//draw left dash line  
		end.x=tl_x;  
		line(img,start,end,color,thickness);       
	}  
  
  
	for (int i=0;i<nCountY;i++)  
	{    
		start.x=tl_x+w;  
		start.y=tl_y+i*totallength;  
		end.y=tl_y+(i+1)*totallength-dashlength;//draw right dash line  
		end.x=tl_x+w;  
		line(img,start,end,color,thickness);       
	}  
  
  
}  


int start_xPos = 0;
int start_yPos = 0;

int end_xPos = 0;
int end_yPos = 0;

bool rightButtonAction = false; 
bool moveAction = false; // action 是否在进行中


bool CpictureCutDlg::rectInSelectSplitImageRects( CvRect r )
{
	
	for( int i = 0 ; i < selectSplitImageRects.size() ; i++ )
	{
			
		CvRect  rect = selectSplitImageRects.at( i );
			
		if(  ( rect.x == r.x ) && ( rect.y == r.y )  )
		{
			return true;
		}
	}

	return false;
}

void CpictureCutDlg::Update(  Point *pt1  , Point *pt2  , bool onlyRectRedraw )
{
	
	cv::Mat duplicateMat;

	if( !onlyRectRedraw ) 
	{
		
		duplicateMat = duplicateSrcMat.clone();
		//cv::Mat duplicateMat = srcMat.clone();

		if( !showSourcePicture )
		{
			duplicateMat = backgroundMat.clone();
		}
		else
		{
		}

		vector<Vec4i> hierarchy;
		RNG rng(0xFFFFFFFF);

		if ( showOutline )
		{
			if( !contours.empty() ) 
			{
				for (int i = 0; i< contours.size(); i++)
				{
					Scalar color;
					int thickness = 1;
					if( isConsHasBeenSelectd( i ) )
					{
						thickness = 1;
						color = CV_RGB(255,0,0 );
					}
					else
					{
						color = Scalar(rng.uniform(1, 254), rng.uniform(1, 254), rng.uniform(1, 254));
					}

					//随机颜色  
					//CvScalar color = CV_RGB(255, 0, 0);
;
					drawContours(duplicateMat, contours, i, color, thickness, 8, hierarchy, 0, Point());

					//test
					
				}
			}
			
		}

		subSrcImageMat = duplicateMat(roiRect ).clone();
		/* 透明度处理 */
		if( showClipImageFlag )
		{
			cv::Mat alphaRoiMat = alphaMat(roiRect).clone();
			subSrcImageMat = subSrcImageMat.mul( alphaRoiMat );
			cv::Mat backgroundMatRoi = backgroundMat(roiRect).clone() .mul( Mat(alphaRoiMat.size(),CV_8UC3,Scalar(1,1,1) ) - alphaRoiMat );
			subSrcImageMat = subSrcImageMat + backgroundMatRoi;
			
		}
			
		if( calibrationFlag )
		{	//画出标定信息
			//蓝色虚线

			//imshow( "show1" , subSrcImageMat );
			//waitKey();

			for( int i = 0 ; i < splitImageRects.size() ; i++ )
			{
				CvRect rect = splitImageRects[i];
			
				rect.x -= roiRect.x;
				rect.y -= roiRect.y;

				if( ( rect.x < 0 ) || (rect.y < 0 ) )
				{
					continue;
				}
				int thickness = 1;
				CvScalar color = CV_RGB(0XCD,0X5F,0X3A);
				if( rectInSelectSplitImageRects(splitImageRects[i]) )
				{
					thickness = 3;
					color = CV_RGB(0XFF,0XFF,0XFF);
				}
				drawDashRect( subSrcImageMat,1,2,&rect,color,thickness );  
				int x = ( rect.x - 5 )> 0 ?   ( rect.x - 5 ): 0 ;
				int y = ( rect.y - 5 )> 0 ?   ( rect.y - 5 ): 0 ;
				String words ; 
				words = format("(%d,%d,%d,%d)",splitImageRects[i].x  , splitImageRects[i].y, splitImageRects[i].width,  splitImageRects[i].height );

				putText( subSrcImageMat, words, Point( x , y ),CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 0) );

			}

			//imshow( "show2" , subSrcImageMat );
			//waitKey();
			
		}
	

	}

	
	
	subSrcImageMat.copyTo(showImageMat);
	//cv::Mat tempShow = showImageMat.clone();
	if( ( pt1 != NULL) && (pt2 != NULL) )
	{
		rectangle(showImageMat, *pt1,*pt2, Scalar(0, 0, 255), 3, 8, 0);
	}
	
	//UpdateWindow( );

	showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat );

	//imshow("src" ,srcMat );
	//imshow("showImageMat" ,showImageMat );
}

void CpictureCutDlg::deleteContous()
{
	vector< vector<Point> >  newContours;//srcMat上的连通域
	for( int i = 0 ; i < this->contours.size();i++)
	{
		if(!isConsHasBeenSelectd ( i ) )
		{
			newContours.push_back( contours[i]);
		}
	}

	contours.clear();
	contours.insert( contours.end(), newContours.begin() , newContours.end() );

	selectList.clear();

	// 重新生成 过滤矩阵（透明度矩阵）
	generateFilterMat();

	if ( showClipImageFlag )
	{
		OnBnClickedBiaoding();
	}
	else
	{
		Update();
	}
}

BOOL CpictureCutDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	 {
		if (pMsg->wParam == VK_DELETE )
		{
			deleteContous();
		}
	 }
	 if (pMsg->message==WM_LBUTTONDOWN )
	 {
		 
		if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd)
		{	
			start_xPos = GET_X_LPARAM(pMsg->lParam);  
			start_yPos = GET_Y_LPARAM(pMsg->lParam);  

			//SetDlgItemText(IDC_STATIC_SHOW1,"BUN1 DOWN");

			moveAction = true;
			rightButtonAction = false;		
		} 
		 
	 }

	  if (pMsg->message==WM_RBUTTONDOWN )
	 {
		  moveAction = true;
		  rightButtonAction = true;
		  if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd)
		  {
			  start_xPos = GET_X_LPARAM(pMsg->lParam);  
			  start_yPos = GET_Y_LPARAM(pMsg->lParam);  

		   //SetDlgItemText(IDC_STATIC_SHOW1,"BUN1 DOWN");
		  } 
	 }
	 
	 if( pMsg->message == WM_RBUTTONUP)
	 {
		rightButtonAction = false;
		moveAction = false;
		
		cv::Point cvPointStart;
		cvPointStart.x = start_xPos;
		cvPointStart.y = start_yPos;

		cv::Point cvPointEnd;
		cvPointEnd.x = end_xPos;
		cvPointEnd.y = end_yPos;
		start_xPos = 0;
		start_yPos = 0;
		end_xPos = 0;
		end_yPos = 0;
		doMultConsSelect( cvPointStart , cvPointEnd );
		doMultClipRectSelect( cvPointStart , cvPointEnd );

		WorkMode = MODE_NULL;
	 }
	 
	 if (pMsg->message==WM_LBUTTONUP)
	 {
		 moveAction = false;
		 end_xPos = GET_X_LPARAM(pMsg->lParam);  
		 end_yPos = GET_Y_LPARAM(pMsg->lParam); 

		 consAdjust( end_xPos , end_yPos );
		 
		 //有down先发生了
		if( ( abs( end_xPos - start_xPos ) < 10 ) || ( abs( end_yPos - start_yPos ) < 10 ) )
		{	//移动太小 // 作为点击操作来处理
				cv::Point cvPoint;

				cvPoint.x = start_xPos;
				cvPoint.y = start_yPos;
					
				start_xPos = 0;
				start_yPos = 0;
				end_xPos = 0;
				end_yPos = 0;
				concourSelect( cvPoint );


			return CDialog::PreTranslateMessage( pMsg );
		}

		if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd && (start_xPos != 0 ) && (start_yPos != 0 ) )
		{
			end_xPos = GET_X_LPARAM(pMsg->lParam);  
			end_yPos = GET_Y_LPARAM(pMsg->lParam);  

		   //SetDlgItemText(IDC_STATIC_SHOW1,"BUN1 DOWN");
			  CRect wndRect;
			  m_showPicture.GetClientRect(&wndRect);

			  //调整ROI rect
			  if( end_xPos >= start_xPos  )
			  {
				  this->roiRect.x += roiRect.width * ( (float)start_xPos / (float)wndRect.Width() ) ;
				  this->roiRect.width = roiRect.width * ( (float)(end_xPos - start_xPos ) / (float)wndRect.Width());

			  }
			  else
			  {
				  this->roiRect.x -= roiRect.width * ( (float)end_xPos / (float)wndRect.Width() ) ;
				  this->roiRect.width = roiRect.width * ( (float)wndRect.Width() / (float)(start_xPos- end_xPos  ) );
			  }
			  if( end_yPos >= start_yPos )
			  {
				  this->roiRect.y += roiRect.height * ( (float)start_yPos / (float)wndRect.Height() ) ;
				  this->roiRect.height = roiRect.height * ( (float)(end_yPos - start_yPos ) / (float)wndRect.Height());
			  }
			  else
			  {
				  this->roiRect.y -= roiRect.height * ( (float)end_yPos / (float)wndRect.Height() ) ;
				  this->roiRect.height = roiRect.height * (  (float)wndRect.Height()/(float)(start_yPos- end_yPos  ) );
			  }

			if( roiRect.x  < 0 ) 
			{
				roiRect.x = 0;
			}

			if( roiRect.y  < 0 ) 
			{
				roiRect.y = 0;
			}
			
			if( roiRect.width > ( srcMat.cols -roiRect.x ) )
			{
				roiRect.width = srcMat.cols -roiRect.x ;

			}
			if( roiRect.height > ( srcMat.rows  -roiRect.y ))
			{
				roiRect.height =  srcMat.rows  -roiRect.y;
			}

			// 变成4的倍数
			if( roiRect.width == 0  )
			{
				 roiRect.width = 100;
				 if( ( roiRect.x + roiRect.width  ) > srcMat.cols )
				 {
					 roiRect.x = srcMat.cols - roiRect.width;
				 }
			}

			if( roiRect.height == 0  )
			{
				 roiRect.height = 100;
				 if( ( roiRect.y + roiRect.height  ) > srcMat.rows )
				 {
					 roiRect.y = srcMat.rows - roiRect.height;
				 }
			}

			roiRect.width >>= 2;  
			roiRect.width <<= 2; 
			
		} 
		else
		{
			start_xPos = 0;
			start_yPos = 0;
		}

		if( ( roiRect.width > 0 )  && ( roiRect.height > 0 ) ) 
		{
			
			Update();
		}
		if( start_xPos != 0 )
		{
			//imshow("show", tempShow );
		}
		start_xPos = 0;
		start_yPos = 0;
		
	 }
	 if(( pMsg->message==WM_MOUSEMOVE  ) && (start_xPos != 0 ) && (start_yPos != 0 )  &&  moveAction )
	 {

		  TRACE( "pMsg->hwnd = 0x%X ,IDC_SHOW_PICTURE = 0x %x \n", pMsg->hwnd ,GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd );
		  if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd)
		  {
		   //SetDlgItemText(IDC_SHOW_PICTURE,"BUN1 UP");
			  end_xPos = GET_X_LPARAM(pMsg->lParam);  
			  end_yPos = GET_Y_LPARAM(pMsg->lParam);  

			  //subSrcImageMat = srcMat(roiRect ).clone();
			  //subSrcImageMat.copyTo( showImageMat );
			  //画出红色矩形框

			  CRect wndRect;
			  m_showPicture.GetClientRect(&wndRect);

			  Rect srcRect; Rect dstRect; Point point; Point rePoint;
			  srcRect.x = 0 ;
			  srcRect.y = 0;
			  srcRect.width = wndRect.Width();
			  srcRect.height = wndRect.Height();

			  dstRect.x = 0 ;
			  dstRect.y = 0;
			  dstRect.width = showImageMat.cols;
			  dstRect.height = showImageMat.rows;

			  Point startPoint;
			  Point endPoint;
			  refect( srcRect , dstRect , Point(start_xPos,start_yPos) , &startPoint );
			  refect( srcRect , dstRect , Point(end_xPos,end_yPos) , &endPoint );
			  //rectangle(showImageMat, startPoint,endPoint, Scalar(0, 0, 255), 3, 8, 0);
			  //showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat );
			  Update( &startPoint ,&endPoint , true );

			  //TRACE( "x = %d  y = %d \n", end_xPos ,end_yPos );
		  } 
	 }

 return CDialog::PreTranslateMessage(pMsg);
}


void CpictureCutDlg::OnClickedShowPicture()
{
	

}

//做 合并标定 rect 选择
void CpictureCutDlg::doMultClipRectSelect( cv::Point start , cv::Point end )
{
	selectSplitImageRects.clear();

	picturePointRefectToRoiPoint( start , start );
	picturePointRefectToRoiPoint( end , end );
	
	int x_start = ( start.x < end.x )?  start.x : end.x ;
	int x_end   = ( start.x < end.x )?  end.x : start.x ;
	int y_start = ( start.y < end.y )?  start.y : end.y ;
	int y_end   = ( start.y < end.y )?  end.y : start.y ;
	
	for( int i = 0 ; i < splitImageRects.size() ; i++ )
	{
			
		CvRect  rect = splitImageRects.at( i );
			
		if(  ( rect.x >= x_start ) && ( rect.x <= x_end ) && ( rect.y >= y_start ) && ( rect.y <= y_end )  )
		{
			selectSplitImageRects.push_back( rect );
		}
	}
}

void CpictureCutDlg::picturePointRefectToRoiPoint( cv::Point in , cv::Point &out  )
{
	CRect wndRect;
	m_showPicture.GetClientRect(&wndRect);

	Rect srcRect; Rect dstRect;  Point rePoint;
	srcRect.x = 0 ;
	srcRect.y = 0;
	srcRect.width = wndRect.Width();
	srcRect.height = wndRect.Height();

	
	dstRect.x = roiRect.x ;
	dstRect.y = roiRect.y;
	dstRect.width = roiRect.width;
	dstRect.height = roiRect.height;

	refect( srcRect , dstRect , in , &out );
}

void CpictureCutDlg::doMultConsSelect(  cv::Point start , cv::Point end )
{
	picturePointRefectToRoiPoint( start , start );
	picturePointRefectToRoiPoint( end , end );


	CvRect roiCutRect;
	roiCutRect.x =  ( start.x < end.x )? start.x : end.x ;
	roiCutRect.y = ( start.y < end.y )? start.y : end.y ;
	roiCutRect.width =  ( start.x < end.x )? (end.x-start.x+1):((start.x-end.x+1));
	roiCutRect.height = ( start.y < end.y )? (end.y-start.y+1):((start.y-end.y+1));

	short keyState = GetKeyState(VK_CONTROL);
	if(! ( keyState <0 ) )
	{// control 键被按下
		selectList.clear();
	}
	
	for( int i = 0 ; i < contours.size();i++ )
	{
		if(  contoursInRoi( contours[i] , roiCutRect ) )
		{
			selectList.push_back(i);
		}
	}

	Update( );
}


void CpictureCutDlg::concourSelect(cv::Point cvPoint)
{
	//获得当前的做标的

	//GetCursorPos(&point);

	
	//GetDlgItem(IDC_SHOW_PICTURE)->ScreenToClient( &point );

	CRect wndRect;
	m_showPicture.GetClientRect(&wndRect);

	Rect srcRect; Rect dstRect;  Point rePoint;
	srcRect.x = 0 ;
	srcRect.y = 0;
	srcRect.width = wndRect.Width();
	srcRect.height = wndRect.Height();

	
	dstRect.x = roiRect.x ;
	dstRect.y = roiRect.y;
	dstRect.width = roiRect.width;
	dstRect.height = roiRect.height;

	refect( srcRect , dstRect , cvPoint , &cvPoint );

	int which = clickFind( cvPoint );

	
	if( GetKeyState(VK_CONTROL)<0 )
	{// control 键被按下

	}
	else
	{
		selectList.clear();
	}

	if( which != -1 )
	{
		selectList.push_back( which );
	}



	Update( );

}


bool contoursInRoi( vector<Point> contour , CvRect roiRect )
{
	for(vector<Point>::iterator it= contour.begin();it!=contour.end();it++)
	{
		if( ( (*it).x > roiRect.x ) && ( (*it).x < ( roiRect.x + roiRect.width ) ) && ( (*it).y > roiRect.y ) && ( (*it).y < ( roiRect.y + roiRect.height ) ))
		{
			// 在 roi 内
		}
		else
		{
			return false;
		}
	}

	return true;
}

//删除掉所有在Rect内的contours
void removeContoursInRoi( vector<vector<Point> >*contours , CvRect  transRect )
{
	for( vector<vector<Point> >::iterator it= contours->begin();it!=contours->end();)
	{
		if( contoursInRoi( *it , transRect ) )
		{
			 it = contours->erase(it);
		}
		else
		{
			it++;
		}
	}

}

void CpictureCutDlg::OnBnClickedCut()
{
	//先删除掉 roi内的所有 contours
	removeContoursInRoi( &contours , roiRect );

	//setRoi
	Pca.SetMat( srcMat( roiRect).clone() );
	Pca.setThreshold( this->cannyThreshold1 , this->cannyThreshold2 );
	Pca.pickOutAllContours();
	vector<vector<Point> > newContours = Pca.getAllContours();
	
	
	//contours 坐标转换
	coordinateTranslate( &newContours , roiRect );

	// 加入到 搜有的contours 中
	contours.insert( contours.end(), newContours.begin() , newContours.end() );
	
	
	Update();
}

//显示原图
void CpictureCutDlg::OnBnShowSourcePicture()
{
	showSourcePicture = !showSourcePicture;

	Update();
}

//显示轮廓
void CpictureCutDlg::OnBnClickedOutline()
{
	if( ( showOutline == FALSE ) && ( contours.size() == 0 ))
	{
		OnBnClickedCut();
	}

	showOutline = !showOutline;

	Update();
}


void CpictureCutDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CpictureCutDlg::OnBnClickedCannyThreshold1Up()
{
	cannyThreshold1 ++;
	UpdateData(FALSE);
	OnBnClickedCut();
}

void CpictureCutDlg::OnBnClickedCannyThreshold1Down()
{
	if( cannyThreshold1 > 1 )
	{
		cannyThreshold1 --;
	}
	UpdateData(FALSE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnBnClickedCannyThreshold2Up()
{
	cannyThreshold2++;
	UpdateData(FALSE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnBnClickedCannyThreshold2Down()
{
	if( cannyThreshold2 > 1 )
	{
		cannyThreshold2--;
	}
	UpdateData(FALSE);
	OnBnClickedCut();
}



void CpictureCutDlg::OnBnClickedCannyThreshold1Up5()
{
	cannyThreshold1 +=5;
	UpdateData(FALSE);
	OnBnClickedCut();
}

void CpictureCutDlg::OnBnClickedCannyThreshold1Down5()
{
	if( cannyThreshold1 > 5 )
	{
		cannyThreshold1 -=5;
	}
	UpdateData(FALSE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnBnClickedCannyThreshold2Up5()
{
	cannyThreshold2 += 5;
	UpdateData(FALSE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnBnClickedCannyThreshold2Down5()
{
	if( cannyThreshold2 > 5 )
	{
		cannyThreshold2 -= 5;
	}
	UpdateData(FALSE);
	OnBnClickedCut();
}



void CpictureCutDlg::OnBnClickedCannyThreshold1Up10()
{
	cannyThreshold1 +=10;
	UpdateData(FALSE);
	OnBnClickedCut();
}

void CpictureCutDlg::OnBnClickedCannyThreshold1Down10()
{
	if( cannyThreshold1 > 10 )
	{
		cannyThreshold1 -=10;
	}
	UpdateData(FALSE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnBnClickedCannyThreshold2Up10()
{
	cannyThreshold2 += 10;
	UpdateData(FALSE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnBnClickedCannyThreshold2Down10()
{
	if( cannyThreshold2 > 10 )
	{
		cannyThreshold2 -= 10;
	}
	UpdateData(FALSE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnEnChangeThreadshold1()
{
	UpdateData(TRUE);
	OnBnClickedCut();
}


void CpictureCutDlg::OnEnChangeThreadshold2()
{
	UpdateData(TRUE);
	OnBnClickedCut();
}

void saveContours(vector<vector<Point>> contours, const char *filename)
{
	FILE* pFile;
	pFile = fopen(filename, "wt");
	if (!pFile)
	{
		return;
	}
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> tempVec = contours[i];
		for (int j = 0; j < tempVec.size(); j++)
		{
			fprintf(pFile, "%d %d ", tempVec[j].x, tempVec[j].y);
		}
		fprintf(pFile, "\n");
	}
	fclose(pFile);
}


void readContours(vector<vector<Point>> *contours, const char *filename)
{
	FILE* pFile;
	int i;
	char s[100], ch;
	pFile = fopen(filename, "rt");
	if (!pFile)
	{
		return;
	}
	bool readFile = TRUE;
	while (readFile)
	{

	}
	int row = 0;
	while (fgets(s, 100, pFile) != NULL)
		row++;
	rewind(pFile);
	int j = 0;
	for (int i = 0; i < row; i++)
	{
		vector<Point> tempVec = (*contours)[i];
		while (fscanf(pFile, "%d %d ", &(((*contours)[i])[j].x), &(((*contours)[i])[j].y)))
		{
			j++;
		}
		
	}
	fclose(pFile);
}

void CpictureCutDlg::saveContoursAndFile(vector<vector<Point>> contours, cv::Mat *src, const char *filename)
{
	FILE* pFile;
	pFile = fopen(filename, "wt");
	if (!pFile)
	{
		return;
	}
	fprintf(pFile, "%d\n", contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> tempVec = contours[i];
		fprintf(pFile, "%d\n", tempVec.size());
		for (int j = 0; j < tempVec.size(); j++)
		{
			fprintf(pFile, "%d %d ", tempVec[j].x, tempVec[j].y);
		}
		fprintf(pFile, "\n");
	}
	
	
	fprintf(pFile, "%d %d\n", src->rows, src->cols);
	for (int i = 0; i < src->rows; i++)
	{
		for (int j = 0; j < src->cols; j++)
		{	
			Vec3b &s = src->at<Vec3b>(i, j);
			fprintf(pFile, "%d %d %d ", s[0], s[1], s[2]);
		}
		fprintf(pFile, "\n");
	}


//	fwrite(&contours, sizeof(vector<Point>), contours.size(), pFile);
//	fwrite(src.data, sizeof(uchar), src.rows * src.cols, pFile);
	fclose(pFile);
}


cv::Mat * CpictureCutDlg::readContoursAndFile(vector<vector<Point>> *contours, const char *filename)
{
	cv::Mat src;
	FILE* pFile;
	pFile = fopen(filename, "rt");
	if (!pFile)
	{
		return NULL;
	}
	int contoursSize;
	fscanf(pFile, "%d", &contoursSize);
	for (int i = 0; i < contoursSize; i++)
	{
		vector<Point> tempVec;
		Point tempPoint;
		int contourSize;
		fscanf(pFile, "%d", &contourSize);
		for (int j = 0; j < contourSize; j++)
		{
			fscanf(pFile, "%d %d ", &tempPoint.x, &tempPoint.y);
			tempVec.push_back(tempPoint);
		}
		contours->push_back(tempVec);
	}
	int matRow, matCol;
	fscanf(pFile, "%d %d ", &matRow, &matCol);
	src.create(matRow, matCol, CV_8UC3);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			Vec3b &s = src.at<Vec3b>(i, j);
			fscanf(pFile, "%d %d %d ", &s[0], &s[1], &s[2]);
		}
	}
	fclose(pFile);
	return &src;
}


 cv::Mat * readContoursAndFile(vector<vector<Point>> *contours, int contoursSize, int matSize, const char *filename)
{
	FILE* pFile;
	pFile = fopen(filename, "rt");
	if (!pFile)
	{
		return NULL;
	}
	fread(contours, sizeof(vector<Point>), contoursSize, pFile);
	//fread(src, sizeof(uchar), matSize, pFile);
	/*
	int i;
	char s[100], ch;
	pFile = fopen(filename, "rt");
	if (!pFile)
	{
		return;
	}
	bool readFile = TRUE;
	while (readFile)
	{

	}
	int row = 0;
	while (fgets(s, 100, pFile) != NULL)
		row++;
	rewind(pFile);
	int j = 0;
	for (int i = 0; i < row; i++)
	{
		vector<Point> tempVec = (*contours)[i];
		while (fscanf(pFile, "%d %d ", &(((*contours)[i])[j].x), &(((*contours)[i])[j].y)))
		{
			j++;
		}
		
	}
	*/

	fclose(pFile);


	return NULL;
}

void CpictureCutDlg::OnBnClickedSaveContours()
{
	CFileDialog dlg(FALSE,("con"),(".con"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,("cons file(*.con)|*.con|"));

	dlg.DoModal();	

	if( dlg.GetPathName().IsEmpty() )
	{
		std::cout << "read data error!" << std::endl;
		return ;
	}
	else
	{
		cv::String cvFileName ;
		cvFileName = dlg.GetPathName().GetBuffer(0);

		saveContoursAndFile( this->contours , &this->srcMat , cvFileName.c_str() );

	}
}


void CpictureCutDlg::OnBnClickedReadContours2()
{
	CFileDialog dlg( TRUE,("con"),(".con"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,("cons file(*.con)|*.con|"));

	dlg.DoModal();	

	if( dlg.GetPathName().IsEmpty() )
	{
		std::cout << "read data error!" << std::endl;
		return ;
	}
	else
	{
		cv::String cvFileName ;
		cvFileName = dlg.GetPathName().GetBuffer(0);

		//readContoursAndFile( &contours , &srcMat , cvFileName.c_str() );

	}
}

void CpictureCutDlg::contourAlphaMask( vector<Point> contour )
{
	int minX = 999999999;
	int maxX = 0;

	map<int ,vector<int> > xMap;  // 按x作为Key放进map
	map<int ,vector<int> > yMap;  // 按y作为Key放进map

	
	for( int i = 0 ; i < contour.size() ;i++ )
	{
		minX = ( contour[i].x < minX ) ? contour[i].x : minX;
		maxX = ( contour[i].x > maxX ) ? contour[i].x : maxX;
		vector<int> yVector;
		
		if( xMap.count( contour[i].x )>0 )
		{
			
		}
		else
		{
			xMap.insert( make_pair(contour[i].x , yVector ));
		}
		xMap[contour[i].x].push_back( contour[i].y );

		vector<int> xVector;
		
		if( yMap.count( contour[i].y )>0 )
		{
			
		}
		else
		{
			yMap.insert( make_pair(contour[i].y , xVector ));
		}
		yMap[contour[i].y].push_back( contour[i].x );
	}

	for( int i = minX ; i <= maxX ; i++ )
	{
		
		vector<int> yVector;
		
		if( xMap.count( i )>0 )
		{
			yVector = xMap[i];
		}

		//yVector 排序
		std::sort(  yVector.begin() , yVector.end() );
		int yMin = yVector[0];
		int yMax = yVector[yVector.size()-1];
		
		for( int m = 0 ; m < yVector.size() ; )
		{
			int start = yVector[m];
			bool inFlag = true;
			m++;	// 跳过开始的点
			int end = start;
			for( int k = m ; k < yVector.size() ; k++ )
			{
				if( yVector[k] == end + 1 )
				{
					end = yVector[k];
					m = k+1;
				}
				else
				{
					// 找到了另一边的开始位置

					/* 刨除掉中间点不在轮廓内的情况 */
					//yVector 排序
					vector<int> xVector;
					int contextValue = 0;
					if( yMap.count( yVector[k]+1 )>0 )
					{
						contextValue = yVector[k]+1;
						xVector = yMap[contextValue];
					}
				
					std::sort(  xVector.begin() , xVector.end() );

					if( ( xVector.size() > 0 ) && ( ( contextValue > xVector[0] ) &&  ( contextValue < xVector[xVector.size() -1 ] ) )  )
					{
						end = yVector[k];
						m = k+1;
					}
					else
					{
						//已经没有点在轮廓内了，do nothing
					}
					
					break; 
				}
			}

	
			
			//确定下一个点是否是在线上的相邻点
			int tryNextEnd = (  m  < yVector.size() ) ?  yVector[m] : yVector[ yVector.size()-1];
			while( ( tryNextEnd ==  ( end +1 ) ) && (  m  < yVector.size() ))
			{
				m++;
				end = tryNextEnd;
				tryNextEnd = ((  m  )< yVector.size() ) ?  yVector[m] : yVector[ yVector.size()-1];
			}

			for( int j = start ; j <= end ; j ++ )
			{
				Vec3b &s = alphaMat.at<Vec3b>(j, i );

				s[0] = 1;
				s[1] = 1;
				s[2] = 1;
			}
		}
		

	}


}
//产生 alpha mat
void CpictureCutDlg:: generateFilterMat()
{
	if( showClipImageFlag )
	{// 全不可见
		vector<Vec4i> hierarchy;
		alphaMat = cv::Mat::zeros( alphaMat.size() , CV_8UC3 );
		if( !contours.empty() ) 
		{
			for (int i = 0; i< contours.size(); i++)
			{
				Scalar color;
				color = CV_RGB(1,1,1 );
;
				drawContours(alphaMat, contours, i, color, -1, 8, hierarchy, 0, Point() );
					
			}
		}
		//生成图像抠图后每个图片的Rect列表
		//splitImageRects = SpliteVisiableImage( alphaMat );

	}
	else
	{
		//splitImageRects.clear();
		alphaMat = Mat(alphaMat.size(),CV_8UC3,Scalar(1,1,1) );
	}
}



bool CpictureCutDlg::isConsHasBeenSelectd( int which )
{
	bool ret = false;

	for( int i = 0 ; i < selectList.size() ; i++ )
	{
		if( selectList[i] == which )
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void CpictureCutDlg::removeConsSelectd( int which )
{
	vector<int>::iterator itor;    
   
	for(itor = selectList.begin();itor!=selectList.end();)  
    {  
        if( which ==*itor)  
        {  
			selectList.erase( itor );  
			break;
        }  
        itor++;  
    }  

}

//根据点找到最近的轮廓（ ）
int CpictureCutDlg::clickFind(Point point)
{
	int ret = -1;

	int isInitValue = 1;
	int minInstance = 10;
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> tempVec = contours[i];
		for (int j = 0; j < tempVec.size(); j++)
		{
			Point tempPoint = tempVec[j];
			int temp = (tempPoint.x - point.x)*(tempPoint.x - point.x) + (tempPoint.y - point.y)*(tempPoint.y - point.y);
			if (temp < minInstance)
			{
				ret = i;
				return ret;
			}
		}

	}
	return ret;
}

void CpictureCutDlg::OnDblclkShowPicture()
{
	// TODO: 在此添加控件通知处理程序代码
}

//获取轮廓的外接矩形
CvRect getConnectAreaRt(vector<Point> *reVec)
{
	// 调用 GetConnectAreaPoint 获得所有和in相连的点的集合
	//将所有点的x放到 vector<int> xVector中 ，并且 调用std::sort排序
	//将所有点的y放到 vector<int> yVector中 ，并且 调用std::sort排序


	//用 （minx,miny） ,(maxx,maxy),形成矩形
	vector<int> xVector;
	vector<int> yVector;

	for (int i = 0; i < reVec->size(); i++)
	{
		xVector.push_back(reVec->at(i).x);
		yVector.push_back(reVec->at(i).y);
	}
	sort(xVector.begin(), xVector.end());
	sort(yVector.begin(), yVector.end());
	int x = xVector.at(0);
	int y = yVector.at(0);
	int width = xVector.at(xVector.size() - 1) - x ;
	int height = yVector.at(yVector.size() - 1) - y ;
	CvRect reCV( x, y, width, height );

	return reCV;
}

//判断旁边的点是否在考虑的点范围内，如果是，则此点也是应该被考虑的
bool isConsiderPoint(cv::Mat &tmpMat, Point in ){


	int line = in.y;
	int col = in.x;


	//左边
	if ((col - 1) >= 0)
	{
		int s = tmpMat.at<uchar>(line , col - 1);
		if (s == 1)
		{
			return true;
		}
	}


	//左上
	if ((line - 1) >= 0 && (col - 1) >= 0)
	{
		int s = tmpMat.at<uchar>(line - 1, col - 1);
		if (s == 1)
		{
			return true;
		}
	}
	


	//上
	if ((line - 1) >= 0)
	{
		int s = tmpMat.at<uchar>(line - 1, col);
		if (s == 1)
		{
			return true;
		}
	}


	//右上
	if ((line - 1 >= 0) && (col + 1) < tmpMat.cols)
	{
		int s = tmpMat.at<uchar>(line - 1, col + 1);
		if (s == 1)
		{
			return true;
		}
	}

	return false;
}




void getConnectArea(cv::Mat &mat, Point in, vector<Point> *reVec){

	cv::Mat tmpMat =  Mat::zeros( mat.rows, mat.cols, CV_8UC1 );
	tmpMat.at<uchar>(in.y,in.x) = 1; 
	reVec->push_back(in);

	for (int i = in.y; i < mat.rows; i++)
	{ // 
		int num = 0;
		for (int j = 0; j < mat.cols; j++)
		{
			Vec3b &s = mat.at<Vec3b>(i, j);
			if (s[0] == 1){
				Point point(j, i);
				if( isConsiderPoint(tmpMat, point ) )
				{
					tmpMat.at<uchar>(i,j) = 1; 
					reVec->push_back( point);
					num ++;
				}
			}
		}
		if (num == 0){
			return;
		}
	}
}



CvRect CpictureCutDlg::GetConnectAreaRect(cv::Mat &mat  , Point in )
{
	vector<Point> reVec;
	
	getConnectArea(mat, in, &reVec);
	CvRect ret = getConnectAreaRt(&reVec);

	return ret;


}
void CpictureCutDlg::OnBnClickedRangeSelect()
{
	
}


//将mat的素有可见区域分割成n个图像
vector<CvRect> CpictureCutDlg::SpliteVisiableImage(cv::Mat mat)
{
	//以x为基础递增循环这个mat( for 循环 先行后列 )
	//如果当前点是 （1,1,1），那么调用 GetConnectAreaPoint 获得其矩形
	// 生成一个输出的mat build 一个 SPLITE_IMAGE_INFO_t 结构，放到输出vector中
	//将上面的矩形内的所有点设置为 (0,0,0)


	//reurn
	vector<CvRect> reVec;
	Point in;
	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			Vec3b &s = mat.at<Vec3b>(i, j);  // at（ 行，列）
			if (s[0] == 1){
				//circle(mat,cvPoint(j ,i),10,CV_RGB(0,255,255),2,8,0);    
				in.x = j;
				in.y = i;
				CvRect reCV = GetConnectAreaRect(mat, in);

				//reset zero data 
				cv::Mat image_roi = mat(reCV);
				cv::Mat zeroMat = cv::Mat::zeros(reCV.height,reCV.width, CV_8UC3);
				zeroMat.copyTo(image_roi);

				reVec.push_back(reCV);
			}
		}
	}


	return reVec;
}


//保存最后的结果

void CpictureCutDlg::saveCalResult( String cvFileDirect , vector<RECT_VECTOR>  RelateList  )
{
	for( int k = 0 ; k < RelateList.size() ; k ++ )
	{
		for( int m = 0 ; m < RelateList[k].rectVec.size() ; m ++ )
		{	
		
			String fileName ;
			fileName = format( "%s/%s_%d.png",cvFileDirect.c_str() , RelateList[k].Name.c_str() , m);
			
			CvRect rect = RelateList[k].rectVec[m] ;
			Mat mat =  Mat::zeros( rect.height,rect.width, CV_8UC4);
			cvtColor( srcMat(rect).clone() , mat ,CV_BGR2BGRA );
			Mat filter = alphaMat(rect).clone();
			for (int i = 0; i < filter.rows; i++)
			{
				for (int j = 0; j < filter.cols; j++)
				{
					Vec4b &mat_s = mat.at<Vec4b>(i, j);
					Vec3b &filter_s = filter.at<Vec3b>(i, j);
					if( filter_s[0] == 0 )
					{
						mat_s[3] = 0;
					}
					else
					{
						mat_s[3] = 255;
					}
					
				}
			}

			//Mat filterMat =  Mat(rect.height,rect.width,CV_8UC4,Scalar(1,1,1 ,1)) ;
			vector<int>compression_params;  
			compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);  
			compression_params.push_back(9);  
			imwrite( fileName , mat, compression_params); 
		}
	}
}

void CpictureCutDlg::OnBnClickedSaveCalResult()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE,(""),("pic"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(""));

	dlg.DoModal();	

	if( dlg.GetPathName().IsEmpty() )
	{
		std::cout << "read GetPathName error!" << std::endl;
		return ;
	}
	else
	{
		String cvFileName ;
		cvFileName = dlg.GetPathName();
		
		for( int k = 0 ; k < splitImageRects.size() ; k++ )
		{
			String fileName ;
			fileName = format( "_%d.png",k);
			fileName = cvFileName + fileName;
			
			CvRect rect = splitImageRects[k];
			Mat mat =  Mat::zeros( rect.height,rect.width, CV_8UC4);
			cvtColor( srcMat(rect).clone() , mat ,CV_BGR2BGRA );
			Mat filter = alphaMat(rect).clone();
			for (int i = 0; i < filter.rows; i++)
			{
				for (int j = 0; j < filter.cols; j++)
				{
					Vec4b &mat_s = mat.at<Vec4b>(i, j);
					Vec3b &filter_s = filter.at<Vec3b>(i, j);
					if( filter_s[0] == 0 )
					{
						mat_s[3] = 0;
					}
					else
					{
						mat_s[3] = 255;
					}
					
				}
			}

			//Mat filterMat =  Mat(rect.height,rect.width,CV_8UC4,Scalar(1,1,1 ,1)) ;
			vector<int>compression_params;  
			compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);  
			compression_params.push_back(9);  
			imwrite( fileName , mat, compression_params);  
		}

	}

	

	
}


//抠图
void CpictureCutDlg::OnBnCutOut()
{
	
	generateFilterMat();
	
	Update();

}


void CpictureCutDlg::OnBnClickedBiaoding()
{
	splitImageRects.clear();
	calibrationFlag = true;

	Mat matClone = alphaMat.clone();
	//test
	/*
	for (int i = 0; i < matClone.rows; i++)
	{
		for (int j = 0; j < matClone.cols; j++)
		{
			Vec3b &filter_s = matClone.at<Vec3b>(i, j);
			if( filter_s[0] == 1 )
			{
				filter_s[1] = 255;
			}	
		}
	}
	*/
	//imshow( " source " , matClone );
	//生成图像抠图后每个图片的Rect列表
	splitImageRects = SpliteVisiableImage(  matClone );

	Update();
	
}

//切换是否显示标定结果
void CpictureCutDlg::OnBnClickedShowBiaodingResult()
{
	calibrationFlag = !calibrationFlag;

	if( showClipImageFlag && ( splitImageRects .size() == 0 ) )
	{
		OnBnClickedBiaoding();
	}

}

//做合并标定矩形的操作
void CpictureCutDlg::OnBnClickedMergeArea()
{
	int x_start = 0;
	int x_end   = 0;
	int y_start = 0;
	int y_end   = 0;

	if( selectSplitImageRects.size() > 1 )
	{

		x_start = selectSplitImageRects.at(0).x;
		x_end = selectSplitImageRects.at(0).x;
		y_start = selectSplitImageRects.at(0).y;
		y_end = selectSplitImageRects.at(0).y;


		vector<CvRect>::iterator itor;
		for( itor = selectSplitImageRects.begin();itor!=selectSplitImageRects.end();)  
		{  
			x_start = ( x_start < (*itor).x ) ? x_start : (*itor).x;
			x_end =   ( x_end > ((*itor).x + (*itor).width)) ? x_end : ((*itor).x + (*itor).width);
			y_start = ( y_start < (*itor).y ) ? y_start : (*itor).y;
			y_end = ( y_end > ((*itor).y + (*itor).height)) ? y_end :  ((*itor).y + (*itor).height);
			vector<CvRect>::iterator selectitor;
			for( selectitor = splitImageRects.begin();selectitor!= splitImageRects.end();)  
			{  
				if( ( itor->x == selectitor->x ) &&  ( itor->y == selectitor->y ) )
				{
					selectitor = splitImageRects.erase( selectitor );
				}
				else
				{
					selectitor++;  
				}

			}  

			itor++;  

			

		}  


		//splitImageRects.erase(
		
		splitImageRects.push_back( CvRect ( x_start ,y_start ,  x_end - x_start+1 , y_end - y_start+1 ) );
		
		Update();

	}

	
}
//找到所有和in 相连接的点，输出到reVec中
void getConnectA(cv::Mat mat, cv::Mat tmpMat, Point in, vector<Point> *reVec){

	for (int i = in.y; i < mat.rows; i++)
	{
		int num = 0;

		for (int j = 0; j < mat.cols; j++)
		{
			int s = mat.at<uchar>(i, j);

			if (s == 0){

				Point point( j, i);

				if (isConsiderPoint(tmpMat, point)){

					tmpMat.at<uchar>(i,j) = 1; 

					reVec->push_back(point);

					num++;

				}




			}

		}

		if (num == 0){

			return;

		}

	}

}

void setPointsValue(cv::Mat mat, vector<Point> *reVec){

	for (int i = 0; i < reVec->size(); i++)
	{
		Point tempP = reVec->at(i);
		mat.at<uchar>(tempP.y, tempP.x) = 255;

	}

}

// 是否是内部点， true 是 false 不是
bool isInsideArea(Point point, cv::Mat mat){

	int x = point.x;
	int y = point.y;


	int x_start = ( (x -1 ) >= 0 )? ( (x -1 ) >= 0 ) : 0 ;
	int x_end   = ( (x +1 ) < mat.cols )? ( ( x +1 ) >= 0 ) : ( mat.cols - 1 ); 
	int y_start = ( (y -1 ) >= 0 )? ( (y -1 ) >= 0 ) : 0 ;
	int y_end   = ( (y +1 ) < mat.rows )? ( ( y +1 ) >= 0 ) :  ( mat.rows - 1 ); 

	for( int i = x_start ; i <= x_end ; i ++ )
	{
		for( int j = y_start ; j <= y_end ; j ++ )
		{
			int s = mat.at<uchar>( y,x - 1);
			if (s == 255)
			{
				return false;
			}
		}
	}

	return true;
}

cv::Mat createCmpMat(cv::Mat mat, Point in, vector<Point> *vecMerge, CvRect reRect){

	cv::Mat tmpMat;

	tmpMat.create(mat.rows, mat.cols, CV_8UC1);


	for (int i = 0; i < tmpMat.rows; i++)
	{
		for (int j = 0; j < tmpMat.cols; j++)
		{
			if (i == in.y && j == in.x){

				tmpMat.at<uchar>(i, j) = 1;

			}
			else
			{
				tmpMat.at<uchar>(i, j) = 0;

			}

		}

	}

	return tmpMat;

}


//去除轮廓中多余的点
void removeRedundanceInContour(vector<Point> * vecMerge){

	if( vecMerge->size() == 0 ) { return ;}
	CvRect reRect = getConnectAreaRt(vecMerge);

	//建 左右都有一个空余的mat
	//cv::Mat src = Mat::zeros(reRect.height + 2, reRect.width + 2, CV_8UC1);

	cv::Mat src = Mat(reRect.height + 2, reRect.width + 2, CV_8UC1,Scalar(255) );
	//vector上都设置为1
	vector<Point> DupVecMerge;
	for (int i = 0; i < vecMerge->size(); i++)
	{
		DupVecMerge.push_back( Point( vecMerge->at(i).x - reRect.x + 1  , vecMerge->at(i).y - reRect.y + 1 ) );
		src.at<uchar>( vecMerge->at(i).y - reRect.y + 1 , vecMerge->at(i).x - reRect.x + 1 ) = 1;
	}

	vector<vector<Point>> cons ;
	cons.push_back( DupVecMerge );
	drawContours( src, cons, -1, Scalar(0 ), -1 );
	/*
	Point point(0, 0);

	vector<Point> reVec;

	reVec.push_back(point);

	cv::Mat tmpMat = createCmpMat(src, point, vecMerge, reRect);

	//找到所有和point 相连接的点，输出到reVec中
	getConnectA(src, tmpMat, point, &reVec);
	imshow( "before setPointsValue" , src );
	//把轮廓外面的点都设置为255
	setPointsValue(src, &reVec);
	*/
	
	imshow( " show" , src );
	int index = 0;

	while (index < vecMerge->size()){

		Point point( vecMerge->at(index).x - reRect.x + 1 , vecMerge->at(index).y - reRect.y + 1 );

		if (isInsideArea(point, src)){

			vecMerge->erase(vecMerge->begin() + index);
		}

		else{

			index++;

		}

	}

}

//对两个轮廓合并进行合并操作
vector<Point> combinationArea( vector<Point> vecOne, vector<Point> vecTwo){

	vector<Point> vecMerge;

	vecMerge.insert(vecMerge.end(), vecOne.begin(), vecOne.end());

	vecMerge.insert(vecMerge.end(), vecTwo.begin(), vecTwo.end());

	removeRedundanceInContour(&vecMerge);

	return vecMerge;

}


#define rectSideLength 10
vector<Point> getRectPointVec(vector<Point> *srcVec, Point in){

	vector<Point> reVec;

	for (int i = 0; i < srcVec->size(); i++)

	{

		Point point = srcVec->at(i);

		if (point.x >= in.x - rectSideLength && point.x <= in.x + rectSideLength && point.y >= in.y - rectSideLength && point.y <= in.y + rectSideLength)

		{

			reVec.push_back(point);

		}

	}

	return reVec;

}


void completePointX(Point beginPoint, Point endPoint, vector<Point> *srcVec){

	int subX = endPoint.x - beginPoint.x;

	int subY = endPoint.y - beginPoint.y;

	int averageY = (int)((float)subY / (float)subX + 0.5);

	for (int i = beginPoint.x + 1; i < endPoint.x; i++)

	{

		Point point(i, (i - beginPoint.x) * averageY + beginPoint.y);

		srcVec->push_back(point);

	}

}







void getBreakPointX(vector<int> xVector, vector<Point> *srcVec){

	int beginX = -1;
	int endX = -1;

	bool isBreakPoint = true;

	for (int i = 0; i < xVector.size()-1; i++)

	{

		if (xVector.at(i + 1) - xVector.at(i) > 1 && isBreakPoint){

			beginX = xVector.at(i);

		}

		else if (xVector.at(i + 1) - xVector.at(i) < 1 && !isBreakPoint){

			endX = xVector.at(i);

		}

	}




	Point beginPoint, endPoint;

	for (int i = 0; i < srcVec->size(); i++)

	{

		if (srcVec->at(i).x == beginX){

			beginPoint = srcVec->at(i);

		}

		if (srcVec->at(i).x == endX){

			endPoint = srcVec->at(i);

		}

	}

	completePointX(beginPoint, endPoint, srcVec);

}



void completePointY(Point beginPoint, Point endPoint, vector<Point> *srcVec){

	int subX = endPoint.x - beginPoint.x;

	int subY = endPoint.y - beginPoint.y;

	int averageX = (int)((float)subX / (float)subY + 0.5);

	for (int i = beginPoint.y + 1; i < endPoint.y; i++)

	{

		Point point((i - beginPoint.y) * averageX + beginPoint.x, i);

		srcVec->push_back(point);

	}

}







void getBreakPointY(vector<int> yVector, vector<Point> *srcVec){

	int beginY = -1;
	int endY = -1;

	bool isBreakPoint = true;

	for (int i = 0; i < yVector.size() -1; i++)

	{

		if (yVector.at(i + 1) - yVector.at(i) > 1 && isBreakPoint){

			beginY = yVector.at(i);

		}

		else if (yVector.at(i + 1) - yVector.at(i) < 1 && !isBreakPoint){

			endY = yVector.at(i);

		}

	}




	Point beginPoint, endPoint;

	for (int i = 0; i < srcVec->size(); i++)

	{

		if (srcVec->at(i).y == beginY){
			beginPoint = srcVec->at(i);

		}

		if (srcVec->at(i).y == endY){
			endPoint = srcVec->at(i);
		}

	}

	completePointY(beginPoint, endPoint, srcVec);

}







void completionRectPointX(vector<Point> *srcVec, Point in){

	vector<Point> reVec = getRectPointVec(srcVec, in);

	vector<int> xVector;

	for (int i = 0; i < reVec.size(); i++)
	{
		xVector.push_back(reVec.at(i).x);
	}
	sort(xVector.begin(), xVector.end());
	getBreakPointX(xVector, srcVec);

}







void completionRectPointY(vector<Point> *srcVec, Point in){

	vector<Point> reVec = getRectPointVec(srcVec, in);

	vector<int> yVector;

	for (int i = 0; i < reVec.size(); i++)
	{
		yVector.push_back(reVec.at(i).y);
	}

	sort(yVector.begin(), yVector.end());

	getBreakPointY(yVector, srcVec);

}


void completionRectPoint(vector<Point> *srcVec){

	for (int i = 0; i < srcVec->size(); i++)
	{

		completionRectPointX(srcVec, srcVec->at(i));

	}

	for (int i = 0; i < srcVec->size(); i++)

	{

		completionRectPointY(srcVec, srcVec->at(i));

	}

}


//多个轮廓合并
void CpictureCutDlg::OnBnClickedMeger()
{
	vector< vector<Point> > select;
	int index = 0;
	for( vector<vector<Point> >::iterator it= contours.begin();it!=contours.end(); index++)
	{
		
		if(  isConsHasBeenSelectd( index )  )
		{
			select.push_back(*it);
			it = contours.erase(it);
		}
		else
		{
			it++;
		}
	}
	selectList.clear();

	vector<Point> vecOne;
	vector<Point> vecTwo;

	if(select.size() > 0 )
	{
		vecOne = select[0];
	}
	for( int i = 1 ; i < select.size() ; i++ )
	{
		vecTwo = select[i];
		vecOne = combinationArea( vecOne , vecTwo );
	}

	if(select.size() > 0 )
	{
		contours.push_back( vecOne );
	}

	selectList.push_back( contours.size() );
	
	Update();
}


//轮廓弥合，在有缺口的地方会被弥合上
void CpictureCutDlg::OnBnClickedFix()
{
	for( int i = 0 ; i < selectList.size() ; i++ )
	{
		completionRectPoint( &contours[i]); 
	}

	Update();
}


bool isRectIntersect(vector<CvRect> * reVec, HEIGHT_INTERVAL * heightValue, CvRect cmpRect){

	int baseRectTopY = heightValue->topY;

	int baseRectBottomY = heightValue->bottomY;




	int cmpRectTopY = cmpRect.y;

	int cmpRectBottomY = cmpRect.y + cmpRect.height;




	if (baseRectTopY <= cmpRectTopY && baseRectBottomY >= cmpRectTopY){

		reVec->push_back(cmpRect);

		if (baseRectBottomY < cmpRectBottomY){

			heightValue->bottomY = cmpRectBottomY;

		}

		return true;

	}

	if (baseRectTopY >= cmpRectTopY && baseRectTopY <= cmpRectBottomY){

		reVec->push_back(cmpRect);

		if (baseRectBottomY < cmpRectBottomY){

			heightValue->topY = cmpRectTopY;

			heightValue->bottomY = cmpRectBottomY;

		}

		else

		{

			heightValue->topY = cmpRectTopY;

		}

		return true;

	}

	return false;

}

bool rectSortByX(const CvRect& obj1,const CvRect& obj2) 
{
	return ( obj1.x < obj2.x );
}

bool rectSortByY(const CvRect& obj1,const CvRect& obj2) 
{
	return ( obj1.y < obj2.y );
}


//获取每个 relative 内的所有的rect
RECT_VECTOR getRelateRectBySort(vector<CvRect> *srcVec, CvRect baseRect){

	int topY = baseRect.y;
	int bottomY = topY + baseRect.height;
	vector<CvRect> reVec;
	reVec.push_back(baseRect);
	HEIGHT_INTERVAL heightValue = { topY, bottomY };
	int index = 0;
	while (index < srcVec->size()){
		CvRect tempRect = srcVec->at(index);
		if (isRectIntersect(&reVec, &heightValue, tempRect)){
			srcVec->erase(srcVec->begin() + index);
		}
		else{
			index++;
		}
	}
	sort( reVec.begin(),reVec.end() , rectSortByX );
	RECT_VECTOR reRectVec = { reVec, heightValue ,"" };
	return reRectVec;

}




vector<RECT_VECTOR> spliteVec(vector<CvRect> srcVec ,String FileName ){

	vector<RECT_VECTOR> reVec;
	
	while (srcVec.size() > 0)
	{
		CvRect baseRect = srcVec.at(0);
		srcVec.erase(srcVec.begin());
		reVec.push_back(getRelateRectBySort(&srcVec, baseRect));
		String name = format("%s_%d", FileName.c_str() , reVec.size() - 1 );
		reVec.at(reVec.size() - 1 ).Name = name;
	}
	return reVec;

}

//输出 layout 文件
void CpictureCutDlg::outputLayoutFile()
{
	

	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE,("xml"),(".xml"),OFN_OVERWRITEPROMPT | OFN_OVERWRITEPROMPT,("xml file(*.xml) |*.xml||"));

	dlg.DoModal();	

	if( dlg.GetPathName().IsEmpty() )
	{
		std::cout << "read GetPathName error!" << std::endl;
		return ;
	}
	else
	{
		String FolderName = dlg.GetFolderPath();//只是路径
		String PahtName = dlg.GetPathName(); //全路径+文件名
		CString FileName = dlg.GetFileName();

		FileName.Replace(".xml","");
		vector<RECT_VECTOR>  RelateList = spliteVec( splitImageRects , FileName.GetBuffer(0) );
	
		saveCalResult( FolderName, RelateList  );
		XlmGenerate  ( PahtName ,RelateList );

	}
}

void CpictureCutDlg::OnBnClickedSaveLayoutFile()
{
	outputLayoutFile();
	
}


inline HRESULT raise_on_failure(HRESULT hResult) {
	if (FAILED(hResult))
		_com_raise_error(hResult);

	return hResult;
}

//格式化 xml文件
void WriteXmlWithFormat(MSXML2::IXMLDOMDocument3* document, IStream* output)
{
	MSXML2::IMXWriterPtr writer(__uuidof(MSXML2::MXXMLWriter60));
	writer->omitXMLDeclaration = VARIANT_FALSE;
	writer->standalone = VARIANT_TRUE;
	writer->indent = VARIANT_TRUE;
	writer->byteOrderMark = VARIANT_FALSE;
	writer->encoding = L"utf-8";

	MSXML2::ISAXContentHandlerPtr ch = writer;
	MSXML2::ISAXErrorHandlerPtr eh = writer;
	MSXML2::ISAXDTDHandlerPtr dh = writer;

	MSXML2::ISAXXMLReaderPtr reader(__uuidof(MSXML2::SAXXMLReader60));

	raise_on_failure(reader->putContentHandler(ch));
	raise_on_failure(reader->putErrorHandler(eh));
	raise_on_failure(reader->putDTDHandler(dh));
	wchar_t lexical_handler[] = L"http://xml.org/sax/properties/lexical-handler";
	wchar_t declaration_handler[] = L"http://xml.org/sax/properties/declaration-handler";
	raise_on_failure(reader->putProperty(reinterpret_cast<unsigned short*>(lexical_handler), _variant_t(writer.GetInterfacePtr())));
	raise_on_failure(reader->putProperty(reinterpret_cast<unsigned short*>(declaration_handler), _variant_t(writer.GetInterfacePtr())));

	writer->output = _variant_t(output);
	raise_on_failure(reader->parse(_variant_t(document)));
}

//打印出单个relative的布局
void CpictureCutDlg::printOutRelativeLayout(  MSXML2::IXMLDOMDocumentPtr pDoc ,MSXML2::IXMLDOMElementPtr pElement , RECT_VECTOR  *Relative )
{
	String commentId =  (char*) (_bstr_t)(pElement->getAttribute( "android:id" ));
	
	float imageW = srcMat.cols;
	float imageH = srcMat.rows;

	MSXML2::IXMLDOMElementPtr pElement1;
	for( int i = 0 ; i < Relative->rectVec.size() ; i++ )
	{
		pElement1 = pDoc -> createElement((_bstr_t)"ImageView");
		// 设置属性
		String Id = format( "%s_%d",commentId.c_str() , i );
		pElement1 -> setAttribute( "android:id" , Id.c_str() );

		//百分比布局的情况下，width/height 必须设置为 0dp
		pElement1 -> setAttribute( "android:layout_width" , "0dp" );
		pElement1 -> setAttribute( "android:layout_height" ,"0dp");
		
		String left = format( "%f%%" , Relative->rectVec[i].x *100 / imageW );
		pElement1 -> setAttribute( "app:layout_marginLeftPercent" , left.c_str() );
		
		String top = format( "%f%%" , ( Relative->rectVec[i].y -  Relative->heightValue.topY + 1)*100/imageH );
		pElement1 -> setAttribute( "app:layout_marginTopPercent" , top.c_str() );

		String w = format( "%f%%" , Relative->rectVec[i].width *100/ imageW );
		pElement1 -> setAttribute( "app:layout_widthPercent" , w.c_str() );
		
		String h = format( "%f%%" , Relative->rectVec[i].height *100/ imageH );
		pElement1 -> setAttribute( "app:layout_heightPercent" , h.c_str() );

		String pic = format( "@drawable/%s_%d",Relative->Name.c_str() ,i ); 
		pElement1 -> setAttribute( "android:background" , pic.c_str() );

		pElement -> appendChild( pElement1 );
	}

}


//打印出所有的 relative 布局
void CpictureCutDlg::printOutRelativeListLayout( MSXML2::IXMLDOMDocumentPtr pDoc , MSXML2::IXMLDOMElementPtr xmlRoot , vector<RECT_VECTOR>  RelateList )
{
	MSXML2::IXMLDOMElementPtr pElement;        // 元素
	String preId = "";
	float imageH = srcMat.rows;
	for( int i = 0 ; i < RelateList.size() ; i++ )
	{
		 // 节点的名称为Book
		pElement = pDoc -> createElement((_bstr_t)"android.support.percent.PercentRelativeLayout");
		// 设置属性
		pElement -> setAttribute( "xmlns:app" ,"http://schemas.android.com/apk/res-auto" );
		String id = format("@+id/%s" , RelateList[i].Name.c_str() ); 
		pElement -> setAttribute( "android:id" ,id.c_str() );
		pElement -> setAttribute( "android:layout_width" ,"match_parent" );
		if( i != 0 )
		{
			pElement -> setAttribute( "android:layout_below" ,preId.c_str() );
		}
		preId = format("@id/%s" , RelateList[i].Name.c_str() ); ;
		
		int h = ( i == 0 ) ? ( RelateList[i].heightValue.bottomY ) : (( RelateList[i].heightValue.bottomY - RelateList[i-1].heightValue.bottomY + 1) );
		pElement -> setAttribute( "android:layout_height" ,"0dp" );

		String height = format( "%f%%" , h *100/ imageH );
		pElement -> setAttribute( "app:layout_heightPercent" , height.c_str() );

		//reset top ,bottom
		RelateList[i].heightValue.topY = RelateList[i].heightValue.bottomY - h;
		printOutRelativeLayout( pDoc , pElement , &RelateList[i] );

		xmlRoot -> appendChild(pElement);
		
	}

}

void CpictureCutDlg::XlmGenerate( String outFile ,vector<RECT_VECTOR>  RelateList )
{
    // TODO: 在此添加控件通知处理程序代码
 
    UpdateData(TRUE);
    MSXML2::IXMLDOMDocumentPtr pDoc;                   // xml文档
    MSXML2::IXMLDOMElementPtr xmlRoot;                 // 根节点
    
    MSXML2::IXMLDOMNodePtr pNode=NULL;                 // 节点
    MSXML2::IXMLDOMProcessingInstructionPtr pProInstruction = NULL;      // xml声明
 
    // 创建DOMDocument对象
    HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));   // MSXML6版本
    if (!SUCCEEDED(hr))
    {
        MessageBox("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库！");
        return;
    }
 
    // 创建xml文档声明：<?xml version="1.0" encoding="utf-8"?>
    pProInstruction = pDoc->createProcessingInstruction((_bstr_t)(char*)"xml", (_bstr_t)(char*)"version=\"1.0\" encoding=\"utf-8\"");
    pDoc->appendChild(pProInstruction);
 
    // 根节点的名称为：LinearLayout
    xmlRoot = pDoc -> createElement((_bstr_t)"android.support.percent.PercentRelativeLayout");
	xmlRoot-> setAttribute( "xmlns:android" ,"http://schemas.android.com/apk/res/android" );
	xmlRoot-> setAttribute( "xmlns:tools" ,"http://schemas.android.com/tools" );
	xmlRoot -> setAttribute( "android:orientation" ,"vertical" );
	xmlRoot-> setAttribute( "android:layout_width" ,"match_parent" );
	xmlRoot-> setAttribute( "android:layout_height" ,"match_parent" );

    pDoc -> PutRefdocumentElement(xmlRoot);
	pDoc->raw_appendChild(xmlRoot,NULL);
 
	printOutRelativeListLayout( pDoc , xmlRoot , RelateList );

	try {
			// 保存到文件。如果不存在就建立,存在就覆盖
			pDoc -> save(outFile.c_str());

			MSXML2::IXMLDOMDocument3Ptr document(__uuidof(MSXML2::DOMDocument60));

			bstr_t const infile( outFile.c_str() );
			bstr_t const outfile( outFile.c_str() );

			document->load( infile );

			IStreamPtr output;
			FileStream::OpenFile( outfile, &output, true);

			WriteXmlWithFormat(document, output);

			
		} catch (_com_error const& e) {
			const TCHAR *  s = e.ErrorMessage();
			std::wcerr << s << std::endl;
	}
}



BOOL CpictureCutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if ( pWnd->m_hWnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd )
	{
		switch( WorkMode )
		{
		case MODE_PEN:
			SetCursor(LoadCursor(NULL,IDC_CROSS )); // 十字
			return TRUE;
		case MODE_RUBBER:
			SetCursor(LoadCursor(NULL,IDC_NO)); //
			return TRUE;
		case MODE_NULL:
		default:
			SetCursor(LoadCursor(NULL,IDC_ARROW )); //设定光标为箭头，就是我们平时用的。
			return TRUE;;
		}
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

//向轮廓中插入一个点
void insertPointInContour(vector<Point> *srcVec, Point in){
	int index = 0;
	int distance = (srcVec->at(0).x - in.x) * (srcVec->at(0).x - in.x) + (srcVec->at(0).y - in.y) * (srcVec->at(0).y - in.y);
	for (int i = 1; i < srcVec->size(); i++)
	{
		Point point = srcVec->at(i);
		if ((point.x - in.x) * (point.x - in.x) + (point.y - in.y) * (point.y - in.y) < distance){
			index = i;
			distance = (point.x - in.x) * (point.x - in.x) + (point.y - in.y) * (point.y - in.y);
		}
	}

	Point tarPoint = srcVec->at(index);
	if (in.x < tarPoint.x){
		srcVec->insert(srcVec->begin() + index, in);
	}
	else if (in.x > tarPoint.x){
		srcVec->insert(srcVec->begin() + index + 1, in);
	}
	else{
		if (in.y < tarPoint.y){
			srcVec->insert(srcVec->begin() + index, in);
		}
		else if (in.y > tarPoint.y){
			srcVec->insert(srcVec->begin() + index + 1, in);
		}
	}

}

//遍历所有轮廓在符合条件的轮廓中添加一点
void addAPointToContour(vector<vector<Point>> *src, Point in){

	for (int i = 0; i < src->size(); i++)
	{

		vector<Point> tempVec = src->at(i);
		int j = 0;
		
		for ( j = 0; j < tempVec.size(); j++)
		{
			Point point = tempVec.at(j);
			if (abs(point.x - in.x) + abs(point.y - in.y) < 5){
				insertPointInContour( &(src->at(i)) , in );
				break;
			}
		}
		if( j < tempVec.size() )
		{
			//removeRedundanceInContour( &(src->at(i)) );
		}
		

	}
}


//删除一点后得到的所有轮廓
vector<vector<Point>> getAfterDelContours(vector<Point> srcVec){
	vector<vector<Point>> reVec;
	vector<Point> tmpVec;
	for (int i = 0; i < srcVec.size(); i++)
	{
		if (i == 0){
			tmpVec.push_back(srcVec.at(i));
		}
		else
		{
			Point pointBefore = srcVec.at(i - 1);
			Point pointAfter = srcVec.at(i);
			if (abs(pointBefore.x - pointAfter.x) >= 1 || abs(pointBefore.y - pointAfter.y) >= 1){
				reVec.push_back(tmpVec);
				tmpVec.clear();
			}
			tmpVec.push_back(srcVec.at(i));
		}
	}
	if (reVec.size() > 1){
		vector<Point> frontVec = reVec.front();
		vector<Point> backVec = reVec.back();
		Point frontVecFirstPoint = frontVec.front();
		Point backVecLastPoint = backVec.back();
		if (abs(frontVecFirstPoint.x - backVecLastPoint.x) <= 1 && abs(frontVecFirstPoint.y - backVecLastPoint.y) <= 1){
			vector<Point> vecMerge;
			vecMerge.insert(vecMerge.end(), backVec.begin(), backVec.end());
			vecMerge.insert(vecMerge.end(), frontVec.begin(), frontVec.end());
			reVec.erase(reVec.begin());
			reVec.erase(reVec.end());
			reVec.push_back(vecMerge);
		}
	}
	return reVec;
}

//遍历所有轮廓在符合条件的轮廓中删除一点
void deleteAPointAtContour(vector<vector<Point>> *src, Point in){
	bool isFind = false;
	for (int i = 0; i < src->size(); i++)
	{
		vector<Point> *tempVec = &src->at(i);
		for (int j = 0; j < tempVec->size(); j++)
		{
			Point point = tempVec->at(j);
			if (point.x == in.x && point.y == in.y){
				tempVec->erase(tempVec->begin() + j);
				/*
				vector<vector<Point>> reBuildCon = getAfterDelContours( (*src)[i] );
				src->erase( src->begin() + i ) ;
				for( int m = 0 ; m < reBuildCon.size() ; m ++ )
				{
					src->push_back( reBuildCon[m] );
				}
				*/
				isFind = true;
				break;
			}
		}
		if (isFind){
			//break;
		}
	}
}



void CpictureCutDlg::consAdjust( int x , int y )
{
	if( ( MODE_PEN == WorkMode ) || ( MODE_RUBBER == WorkMode ) )
	{
		cv::Point in(x , y );
		picturePointRefectToRoiPoint( in , in  );

		if( MODE_PEN == WorkMode )
		{
			addAPointToContour( &contours, in  );
			Update();
		}
		else if( MODE_RUBBER == WorkMode )
		{
			deleteAPointAtContour( &contours, in );
			Update();
		}
	}

}