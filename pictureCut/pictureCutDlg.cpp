
// pictureCutDlg.cpp : ʵ���ļ�
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
void coordinateTranslate( vector<vector<Point> >*contours , CvRect  transRect );
void refect( Rect srcRect, Rect dstRect, Point point, Point *rePoint);
bool contoursInRoi( vector<Point> contour , CvRect roiRect );

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CpictureCutDlg �Ի���




CpictureCutDlg::CpictureCutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpictureCutDlg::IDD, pParent)
{
	//showImageMat = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	cannyThreshold1 = 80;
	cannyThreshold2 = 255;
	showBuffer = NULL;
	bitBuffer = NULL;
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
	DDX_Control(pDX, IDC_SELECT, m_selectbtn);
	DDX_Control(pDX, IDC_RANGE_SELECT, m_rangeSelectbtn);
}

BEGIN_MESSAGE_MAP(CpictureCutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START2, &CpictureCutDlg::OnBnClickedStart2)
	ON_BN_CLICKED(IDC_ZOOM_UP, &CpictureCutDlg::OnBnClickedZoomUp)
	ON_BN_CLICKED(IDC_ZOOM_DOWN, &CpictureCutDlg::OnBnClickedZoomDown)
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
	ON_BN_CLICKED(IDC_ZOOM_DOWN4, &CpictureCutDlg::OnBnClickedOutline)
	ON_BN_CLICKED(IDC_SAVE_CONTOURS, &CpictureCutDlg::OnBnClickedSaveContours)
	ON_BN_CLICKED(IDC_READ_CONTOURS2, &CpictureCutDlg::OnBnClickedReadContours2)
	ON_BN_CLICKED(IDC_SELECT, &CpictureCutDlg::OnBnCutOut)
	ON_STN_DBLCLK(IDC_SHOW_PICTURE, &CpictureCutDlg::OnDblclkShowPicture)
	ON_BN_CLICKED(IDC_RANGE_SELECT, &CpictureCutDlg::OnBnClickedRangeSelect)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CpictureCutDlg ��Ϣ�������

BOOL CpictureCutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//hBmp = AfxGetApp()->LoadIcon( IDR_MAINFRAME );

	m_selectbtn.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_SELECT)));   
	
	m_rangeSelectbtn.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_RANGE_BITMAP))); 
	
	//m_selectbtn.SetBitmap(hBmp);

	selectList.empty();

	showSourcePicture = TRUE;
	dupAndZeorInitFlag = false;
	showOutline = FALSE;
	cutOutFlag = false;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpictureCutDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpictureCutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void OnMouseAction(int event,int x,int y,int flags,void *ustc);  //���ص��¼�����  
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

		roiRect.x = 0;
		roiRect.y = 0;
		roiRect.height = srcMat.rows;
		roiRect.width = srcMat.cols;

		{
			dupAndZeorInitFlag = true;
			//cvtColor( srcMat , duplicateSrcMat,CV_BGR2BGRA );
			duplicateSrcMat = srcMat.clone();
			backgroundMat = Mat::zeros( srcMat.size(), CV_8UC3); // ��ʼ������ɫΪ��ɫ
			alphaMat = Mat( srcMat.size(),CV_8UC3,Scalar(1,1,1)); // ��ʼ�� alpha����
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

//�ڴ��е�ͼ�����ݿ�������Ļ�� 

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
		pWnd->GetClientRect(drect); //(drect); (&drect); ���ַ�ʽ���ɣ���Ȼ





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
		bitMapinfo->bmiHeader.biHeight = -img.rows; //����߶�Ϊ���ģ�λͼ����ʼλ�������½ǡ�����߶�Ϊ������ʼλ�������Ͻǡ� 
		bitMapinfo->bmiHeader.biWidth = img.cols; 
		bitMapinfo->bmiHeader.biPlanes = 1; // Ŀ���豸�ļ��𣬱���Ϊ1 
		bitMapinfo->bmiHeader.biBitCount = ichannels *8; // ÿ�����������λ����������1(˫ɫ), 4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ 
		bitMapinfo->bmiHeader.biCompression = BI_RGB; //λͼѹ�����ͣ������� 0(��ѹ��), 1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ 
		bitMapinfo->bmiHeader.biSizeImage = 0; // λͼ�Ĵ�С�����ֽ�Ϊ��λ 
		bitMapinfo->bmiHeader.biXPelsPerMeter = 0; // λͼˮƽ�ֱ��ʣ�ÿ�������� 
		bitMapinfo->bmiHeader.biYPelsPerMeter = 0; // λͼ��ֱ�ֱ��ʣ�ÿ�������� 
		bitMapinfo->bmiHeader.biClrUsed = 0; // λͼʵ��ʹ�õ���ɫ���е���ɫ�� 
		bitMapinfo->bmiHeader.biClrImportant = 0; // λͼ��ʾ��������Ҫ����ɫ��

		if(ichannels == 1) 
		{ 
			for(int i=0; i<256; i++) 
			{ //��ɫ��ȡֵ��Χ (0-255) 
				bitMapinfo->bmiColors[i].rgbBlue =bitMapinfo->bmiColors[i].rgbGreen =bitMapinfo->bmiColors[i].rgbRed =(BYTE) i; 
			}

			bitMapinfo->bmiHeader.biClrUsed = 256; // λͼʵ��ʹ�õ���ɫ���е���ɫ�� 
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
		imgHeight = img.rows; //ͼ��߶� 
	}
	StretchDIBits(	hDC, 
					0, 
					0, 
					drect.right, //��ʾ���ڿ�� 
					drect.bottom, //��ʾ���ڸ߶� 
					0, 
					0, 
					imgWidth, //ͼ���� 
					imgHeight, //ͼ��߶� 
					showBuffer, 
					bitMapinfo, 
					DIB_RGB_COLORS, 
					SRCCOPY 
	);

	//delete []showBuffer;
	//delete []bitBuffer;

#endif

}



//�Ŵ�ͼƬ
void CpictureCutDlg::OnBnClickedZoomUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	roiRect.width /= 2;
	roiRect.height /= 2;

	Update();

	/*
	namedWindow("loveLena", CV_WINDOW_AUTOSIZE); 
    //��ʾͼƬ������㲻���ⴰ�ڴ�С�ɱ䣬����ֱ��ע�͵���һ�䡣��Ϊimshow����ֱ�Ӵ�������
    imshow("loveLena", showImageMat);
    waitKey(); //�ȴ�����
	*/
 
}


void CpictureCutDlg::OnBnClickedZoomDown()
{
	roiRect.x -= (roiRect.width/2);
	roiRect.y -= (roiRect.height/2);
	
	roiRect.width *= 2;
	roiRect.height *= 2;

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

	// ���4�ı���
	roiRect.width >>= 2;  
	roiRect.width <<= 2; 

	Update();
	//imshow("view", showImageMat);
}


void CpictureCutDlg::OnThemechangedShowPicture(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �ù���Ҫ��ʹ�� Windows XP ����߰汾��
	// ���� _WIN32_WINNT ���� >= 0x0501��
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

int start_xPos = 0;
int start_yPos = 0;

int end_xPos = 0;
int end_yPos = 0;

bool rightButtonAction = false; 
bool moveAction = false; // action �Ƿ��ڽ�����



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
					int thickness = 2;
					if( isConsHasBeenSelectd( i ) )
					{
						thickness = 5;
						color = CV_RGB(255,0,0 );
					}
					else
					{
						color = Scalar(rng.uniform(1, 254), rng.uniform(1, 254), rng.uniform(1, 254));
					}

					//�����ɫ  
					//CvScalar color = CV_RGB(255, 0, 0);
;
					drawContours(duplicateMat, contours, i, color, thickness, 8, hierarchy, 0, Point());

					//test
					
				}
			}
		}


		subSrcImageMat = duplicateMat(roiRect ).clone();
		/* ͸���ȴ��� */
		{
			cv::Mat alphaRoiMat = alphaMat(roiRect).clone();
			subSrcImageMat = subSrcImageMat.mul( alphaRoiMat );
			cv::Mat backgroundMatRoi = backgroundMat(roiRect).clone() .mul( Mat(alphaRoiMat.size(),CV_8UC3,Scalar(1,1,1) ) - alphaRoiMat );
			subSrcImageMat = subSrcImageMat + backgroundMatRoi;

			
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
	vector< vector<Point> >  newContours;//srcMat�ϵ���ͨ��
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

	Update();
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
	 }
	 
	 if (pMsg->message==WM_LBUTTONUP)
	 {
		 moveAction = false;
		 end_xPos = GET_X_LPARAM(pMsg->lParam);  
		 end_yPos = GET_Y_LPARAM(pMsg->lParam);  
		 //��down�ȷ�����
		if( ( abs( end_xPos - start_xPos ) < 10 ) || ( abs( end_yPos - start_yPos ) < 10 ) )
		{	//�ƶ�̫С // ��Ϊ�������������
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

			  //����ROI rect
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

			// ���4�ı���
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
			  //������ɫ���ο�

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


void CpictureCutDlg::doMultConsSelect(  cv::Point start , cv::Point end )
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

	refect( srcRect , dstRect , start , &start );
	refect( srcRect , dstRect , end , &end );

	CvRect roiCutRect;
	roiCutRect.x =  ( start.x < end.x )? start.x : end.x ;
	roiCutRect.y = ( start.y < end.y )? start.y : end.y ;
	roiCutRect.width =  ( start.x < end.x )? (end.x-start.x+1):((start.x-end.x+1));
	roiCutRect.height = ( start.y < end.y )? (end.y-start.y+1):((start.y-end.y+1));

	short keyState = GetKeyState(VK_CONTROL);
	if(! ( keyState <0 ) )
	{// control ��������
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
	//��õ�ǰ�������

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
	{// control ��������

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
			// �� roi ��
		}
		else
		{
			return false;
		}
	}

	return true;
}

//ɾ����������Rect�ڵ�contours
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
	//��ɾ���� roi�ڵ����� contours
	removeContoursInRoi( &contours , roiRect );

	//setRoi
	Pca.SetMat( srcMat( roiRect).clone() );
	Pca.setThreshold( this->cannyThreshold1 , this->cannyThreshold2 );
	Pca.pickOutAllContours();
	vector<vector<Point> > newContours = Pca.getAllContours();
	
	
	//contours ����ת��
	coordinateTranslate( &newContours , roiRect );

	// ���뵽 ���е�contours ��
	contours.insert( contours.end(), newContours.begin() , newContours.end() );
	
	
	Update();
}

//��ʾԭͼ
void CpictureCutDlg::OnBnShowSourcePicture()
{
	showSourcePicture = !showSourcePicture;

	Update();
}

//��ʾ����
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	map<int ,vector<int> > xMap;  // ��x��ΪKey�Ž�map
	map<int ,vector<int> > yMap;  // ��y��ΪKey�Ž�map

	
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

		//yVector ����
		std::sort(  yVector.begin() , yVector.end() );
		int yMin = yVector[0];
		int yMax = yVector[yVector.size()-1];
		
		for( int m = 0 ; m < yVector.size() ; )
		{
			int start = yVector[m];
			bool inFlag = true;
			m++;	// ������ʼ�ĵ�
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
					// �ҵ�����һ�ߵĿ�ʼλ��

					/* �ٳ����м�㲻�������ڵ���� */
					//yVector ����
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
						//�Ѿ�û�е����������ˣ�do nothing
					}
					
					break; 
				}
			}

	
			
			//ȷ����һ�����Ƿ��������ϵ����ڵ�
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

//��ͼ
void CpictureCutDlg::OnBnCutOut()
{
	
	cutOutFlag = !cutOutFlag;

	
	if( cutOutFlag )
	{// ȫ���ɼ�
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

	}
	else
	{
		alphaMat = Mat(alphaMat.size(),CV_8UC3,Scalar(1,1,1) );
	}

	
	Update();

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

//���ݵ��ҵ������������ ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CpictureCutDlg::OnBnClickedRangeSelect()
{
	
}

