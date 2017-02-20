
// pictureCutDlg.h : 头文件
//

#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"  
#include "algorithm\PictureCutAlgorithm.h"

#pragma once
using namespace cv;

// CpictureCutDlg 对话框
typedef struct cons
{
	bool select ;
	vector<Point>  pointSet;
}CONTOURS_t;



//CONTOURS_t contoursArray[1024];

class CpictureCutDlg : public CDialogEx
{
// 构造
public:
	CpictureCutDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CpictureCutDlg();

// 对话框数据
	enum { IDD = IDD_PICTURECUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
private:
	cv::Mat srcMat;
	bool dupAndZeorInitFlag ;
	cv::Mat duplicateSrcMat;
	cv::Mat backgroundMat; // 背景色
	cv::Mat alphaMat; // A 矩阵 ， 0 透明， 1，不透明
	cv::Mat subSrcImageMat; // 缩放或指定rect的图像

	cv::Mat showImageMat; // 用于输出显示的mat ，在srcMat上叠加自定义指示

	CvRect  roiRect ; // 限制显示区域的 
	vector< vector<Point> >  contours ; // srcMat上的连通域 

	vector< int > selectList;// 被选中的contour

	PictureCutAlgorithm Pca;

	//显示原图
	BOOL showSourcePicture;
	//显示轮廓
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

	// 生成的消息映射函数
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
