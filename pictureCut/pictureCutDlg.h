
// pictureCutDlg.h : ͷ�ļ�
//

#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"  
#include "algorithm\PictureCutAlgorithm.h"

#pragma once
using namespace cv;

// CpictureCutDlg �Ի���
typedef struct cons
{
	bool select ;
	vector<Point>  pointSet;
}CONTOURS_t;



//CONTOURS_t contoursArray[1024];

class CpictureCutDlg : public CDialogEx
{
// ����
public:
	CpictureCutDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CpictureCutDlg();

// �Ի�������
	enum { IDD = IDD_PICTURECUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
private:
	cv::Mat srcMat;
	bool dupAndZeorInitFlag ;
	cv::Mat duplicateSrcMat;
	cv::Mat backgroundMat; // ����ɫ
	cv::Mat alphaMat; // A ���� �� 0 ͸���� 1����͸��
	cv::Mat subSrcImageMat; // ���Ż�ָ��rect��ͼ��

	cv::Mat showImageMat; // ���������ʾ��mat ����srcMat�ϵ����Զ���ָʾ

	CvRect  roiRect ; // ������ʾ����� 
	vector< vector<Point> >  contours ; // srcMat�ϵ���ͨ�� 

	vector< int > selectList;// ��ѡ�е�contour

	PictureCutAlgorithm Pca;

	//��ʾԭͼ
	BOOL showSourcePicture;
	//��ʾ����
	BOOL showOutline;

	//for onpaint 
	BYTE *bitBuffer ; 
	BYTE *showBuffer ;
	BITMAPINFO *bitMapinfo ;
	CRect drect; 
	int imgWidth ;
	int imgHeight ;
	HDC hDC;

	bool cutOutFlag;
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart2();

private:
	//void showMatImgToWnd(CWnd* pWnd, const cv::Mat& img);
	void Update( Point *pt1 = NULL, Point *pt2 = NULL ,bool onlyRectRedraw = false );
	void showMatImgToWnd( CWnd* pWnd, cv::Mat img ,bool forceUpdate = true );
public:
	afx_msg void OnBnClickedZoomUp();
	afx_msg void OnBnClickedZoomDown();
	afx_msg void OnThemechangedShowPicture(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_showPicture;
	afx_msg void OnClickedShowPicture();
	afx_msg void OnBnClickedCut();

	afx_msg void OnBnShowSourcePicture();
	afx_msg void OnEnChangeEdit1();
	bool isConsHasBeenSelectd( int which );
	void removeConsSelectd( int which );
	int clickFind(Point point);	// return -1 is invalid
	void concourSelect( cv::Point cvPoint );
	void doMultConsSelect( cv::Point start , cv::Point end );
	void deleteContous();
	void saveContoursAndFile(vector<vector<Point>> contours, cv::Mat *src, const char *filename);
	cv::Mat * readContoursAndFile(vector<vector<Point>> *contours, const char *filename);
	void contourAlphaMask( vector<Point> contour );
	// cvCanny threshold1
	UINT cannyThreshold1;
	// cvCanny threshold2
	UINT cannyThreshold2;
	afx_msg void OnBnClickedCannyThreshold1Up();
	afx_msg void OnBnClickedCannyThreshold1Down();
	afx_msg void OnBnClickedCannyThreshold2Up();
	afx_msg void OnBnClickedCannyThreshold2Down();
	afx_msg void OnBnClickedCannyThreshold1Up5();
	afx_msg void OnBnClickedCannyThreshold1Down5();
	afx_msg void OnBnClickedCannyThreshold2Up5();
	afx_msg void OnBnClickedCannyThreshold2Down5();
	afx_msg void OnBnClickedCannyThreshold1Up10();
	afx_msg void OnBnClickedCannyThreshold1Down10();
	afx_msg void OnBnClickedCannyThreshold2Up10();
	afx_msg void OnBnClickedCannyThreshold2Down10();

	afx_msg void OnEnChangeThreadshold1();
	afx_msg void OnEnChangeThreadshold2();
	afx_msg void OnBnClickedOutline();
	afx_msg void OnBnClickedSaveContours();
	afx_msg void OnBnClickedReadContours2();
	CButton m_selectbtn;
	afx_msg void OnBnCutOut();
	afx_msg void OnDblclkShowPicture();
	afx_msg void OnBnClickedRangeSelect();
	CButton m_rangeSelectbtn;
};
