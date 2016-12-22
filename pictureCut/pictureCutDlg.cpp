
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
}

BEGIN_MESSAGE_MAP(CpictureCutDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START2, &CpictureCutDlg::OnBnClickedStart2)
	ON_BN_CLICKED(IDC_ZOOM_UP, &CpictureCutDlg::OnBnClickedZoomUp)
	ON_BN_CLICKED(IDC_ZOOM_DOWN, &CpictureCutDlg::OnBnClickedZoomDown)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SHOW_PICTURE, &CpictureCutDlg::OnThemechangedShowPicture)
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

		
		subSrcImageMat = srcMat(roiRect ).clone();
		showImageMat = subSrcImageMat.clone();
		showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat );
	}


	
}

void CpictureCutDlg::showMatImgToWnd(CWnd* pWnd, const cv::Mat& img)
{ 
    if(img.empty()) 
        return; 
	 static BITMAPINFO *bitMapinfo = NULL;
	 static bool First=TRUE;
	 if(First)
	 {  
		  BYTE *bitBuffer = new BYTE[40+4*256];//����һ���ڴ�����
		  if(bitBuffer == NULL)
		  { 
		   return;
		  }
		  First=FALSE;
		  memset(bitBuffer, 0, 40+4*256);
		  bitMapinfo = (BITMAPINFO *)bitBuffer;
		  bitMapinfo->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
		  bitMapinfo->bmiHeader.biPlanes   = 1;   
		  for(int i=0; i<256; i++)
		  { //��ɫ��ȡֵ��Χ (0-255)
		   bitMapinfo->bmiColors[i].rgbBlue  =bitMapinfo->bmiColors[i].rgbGreen =bitMapinfo->bmiColors[i].rgbRed   =(BYTE) i;
		  } 
	  }
	  bitMapinfo->bmiHeader.biHeight = -img.rows;   
	  bitMapinfo->bmiHeader.biWidth = img.cols;
	  bitMapinfo->bmiHeader.biBitCount= img.channels() *8;    
 
	 CRect drect;       
	 pWnd->GetClientRect(drect);    //pWndָ��CWnd���һ��ָ�� 
	 CClientDC dc(pWnd);
	 HDC hDC =dc.GetSafeHdc();                  //HDC��Windows��һ���������ͣ����豸���������
	 SetStretchBltMode(hDC, COLORONCOLOR);    
	 
	 StretchDIBits(hDC,
		 0,
		 0,
		 drect.Width(),     
		 drect.Height(),    		 
		 0,
		 0,
		 img.cols,     //ͼ����
		 img.rows,     //ͼ��߶�
		 img.data,   
		 bitMapinfo,   
		 DIB_RGB_COLORS, 
		 SRCCOPY
		  );
}


//�Ŵ�ͼƬ
void CpictureCutDlg::OnBnClickedZoomUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	roiRect.width /= 2;
	roiRect.height /= 2;

	subSrcImageMat = srcMat(roiRect ).clone();
	showImageMat = subSrcImageMat.clone();
	showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat );

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
	
	if( roiRect.width > srcMat.cols )
	{
		roiRect.width = srcMat.cols;

	}
	if( roiRect.height > srcMat.rows )
	{
		roiRect.height = srcMat.rows;
	}

	
	subSrcImageMat = srcMat(roiRect ).clone();
	showImageMat = subSrcImageMat.clone();
	showMatImgToWnd( GetDlgItem( IDC_SHOW_PICTURE  ) , showImageMat );
	
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

BOOL CpictureCutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	 if (pMsg->message==WM_LBUTTONDOWN)
	 {
		  if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd)
		  {
			  start_xPos = GET_X_LPARAM(pMsg->lParam);  
			  start_yPos = GET_Y_LPARAM(pMsg->lParam);  

		   //SetDlgItemText(IDC_STATIC_SHOW1,"BUN1 DOWN");
		  } 
	 }

	 if (pMsg->message==WM_LBUTTONUP)
	 {
		if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd)
		{
			  end_xPos = GET_X_LPARAM(pMsg->lParam);  
			  end_yPos = GET_Y_LPARAM(pMsg->lParam);  

		   //SetDlgItemText(IDC_STATIC_SHOW1,"BUN1 DOWN");
		} 
	 }
	 if(pMsg->message==WM_MOUSEMOVE)
	 {
		  if (pMsg->hwnd == GetDlgItem(IDC_SHOW_PICTURE)->m_hWnd)
		  {
		   //SetDlgItemText(IDC_SHOW_PICTURE,"BUN1 UP");
			  end_xPos = GET_X_LPARAM(pMsg->lParam);  
			  end_yPos = GET_Y_LPARAM(pMsg->lParam);  
		  } 
	 }

 return CDialog::PreTranslateMessage(pMsg);
}
	