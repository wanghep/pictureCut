
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

void refect( Rect srcRect, Rect dstRect, Point point, Point *rePoint);
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
}

void CpictureCutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_PICTURE, m_showPicture);
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

	showSourcePicture = TRUE;

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
			showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat );
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
	CFileDialog dlg(TRUE,("png"),(".png"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,("PNG file(*.png)|*.png||"));

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


void CpictureCutDlg::showMatImgToWnd( CWnd* pWnd, cv::Mat img )
//void BitMatToWnd(CWnd* pWnd, cv::Mat img, CRect *Roi)
{

#if 1
	if(img.empty()) 
		return;

	CRect drect; 
	pWnd->GetClientRect(drect); //(drect); (&drect); ���ַ�ʽ���ɣ���Ȼ

	CClientDC dc(pWnd); 
	HDC hDC =dc.GetSafeHdc();

	//�ڴ��е�ͼ�����ݿ�������Ļ�� 
	BYTE *bitBuffer = NULL; 
	BITMAPINFO *bitMapinfo = NULL;

	int ichannels =img.channels(); 
	int depth = img.depth();
	bool continous = img.isContinuous();
	if( ichannels == 1) 
	{ 
		bitBuffer = new BYTE[40+4*256]; 
	} 
	else if( ichannels == 3) 
	{ 
		bitBuffer = new BYTE[sizeof(BITMAPINFO)]; 
	} 
	else 
	{ 
		return; 
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

	BYTE *showBuffer = new byte[ img.cols * img.rows * ichannels ]; 

	unsigned char * move = img.data;
	for( int j = 0 ; j <  img.rows ; j++ )	
	{
		memcpy( showBuffer + j * img.cols * ichannels ,  move ,  img.cols * ichannels );
		move += img.step;
		
	}
	//saveImage( showBuffer , img.cols * ichannels , img.rows);
	
	StretchDIBits(	hDC, 
					0, 
					0, 
					drect.right, //��ʾ���ڿ�� 
					drect.bottom, //��ʾ���ڸ߶� 
					0, 
					0, 
					img.cols, //ͼ���� 
					img.rows, //ͼ��߶� 
					showBuffer, 
					bitMapinfo, 
					DIB_RGB_COLORS, 
					SRCCOPY 
	);

	delete []showBuffer;
	delete []bitBuffer;

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

void CpictureCutDlg::Update(  Point *pt1  , Point *pt2  )
{
	cv::Mat duplicateMat = srcMat.clone();

	if( !showSourcePicture )
	{
		duplicateMat = Mat::zeros( srcMat.size(), CV_8UC3);
	}

	vector<Vec4i> hierarchy;
	RNG rng(0xFFFFFFFF);

	if( !contours.empty() ) 
	{
		for (int i = 0; i< contours.size(); i++)
		{
			//�����ɫ  
			//CvScalar color = CV_RGB(255, 0, 0);
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(duplicateMat, contours, i, color, 2, 8, hierarchy, 0, Point());
		}
	}

	subSrcImageMat = duplicateMat(roiRect ).clone();
	subSrcImageMat.copyTo(showImageMat);
	//cv::Mat tempShow = showImageMat.clone();
	if( ( pt1 != NULL) && (pt2 != NULL) )
	{
		rectangle(showImageMat, *pt1,*pt2, Scalar(0, 0, 255), 3, 8, 0);
	}
	showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat );

}

BOOL CpictureCutDlg::PreTranslateMessage(MSG* pMsg)
{
	 if (pMsg->message==WM_LBUTTONDOWN )
	 {
		  moveAction = true;
		  rightButtonAction = false;
		  if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd)
		  {
			  start_xPos = GET_X_LPARAM(pMsg->lParam);  
			  start_yPos = GET_Y_LPARAM(pMsg->lParam);  

		   //SetDlgItemText(IDC_STATIC_SHOW1,"BUN1 DOWN");
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
	 }
	 
	 if (pMsg->message==WM_LBUTTONUP)
	 {
		 moveAction = false;
		 //��down�ȷ�����
		if( ( abs( end_xPos - start_xPos ) < 10 ) || ( abs( end_yPos - start_yPos ) < 10 ) )
		{	//�ƶ�̫С
			start_xPos = 0;
			start_yPos = 0;
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
			  Update( &startPoint ,&endPoint );

			  TRACE( "x = %d  y = %d \n", end_xPos ,end_yPos );
		  } 
	 }

 return CDialog::PreTranslateMessage(pMsg);
}




void CpictureCutDlg::OnClickedShowPicture()
{

}


void CpictureCutDlg::OnBnClickedCut()
{
	//setRoi
	Pca.pickOutAllContours();
	contours =  Pca.getAllContours();
	Update();
}

//��ʾԭͼ
void CpictureCutDlg::OnBnShowSourcePicture()
{
	showSourcePicture = !showSourcePicture;

	Update();
}
