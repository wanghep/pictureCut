
// pictureCutDlg.h : ͷ�ļ�
//

#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"  
#include "algorithm\PictureCutAlgorithm.h"

#pragma once
using namespace cv;
#import <msxml6.dll>

// CpictureCutDlg �Ի���
typedef struct cons
{
	bool select ;
	vector<Point>  pointSet;
}CONTOURS_t;


typedef struct

{

	int topY;

	int bottomY;


}HEIGHT_INTERVAL;

typedef struct

{
	vector<CvRect> rectVec;
	HEIGHT_INTERVAL heightValue;
	String Name;
}RECT_VECTOR;

typedef enum
{
	MODE_NULL = 0 ,
	MODE_PEN  = 1 ,
	MODE_RUBBER = 2
}CONS_WORK_MODE_e;

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
	CONS_WORK_MODE_e WorkMode ;
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

	vector<CvRect> splitImageRects; // ��ͼ���ͼ��ÿ��ͼƬ��Rect�б�
	vector<CvRect> selectSplitImageRects; // ��ѡ�е�Rect�б�


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

	bool showClipImageFlag; // �Ƿ���ʾ�궨���
	bool calibrationFlag;
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
	afx_msg void OnBnClickedConPen();
	afx_msg void OnBnClickedRubble();
	afx_msg void OnThemechangedShowPicture(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_showPicture;
	afx_msg void OnClickedShowPicture();
	afx_msg void OnBnClickedCut();

	afx_msg void OnBnShowSourcePicture();
	afx_msg void OnEnChangeEdit1();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void saveCalResult( String cvFileDirect  , vector<RECT_VECTOR>  RelateList );
	bool rectInSelectSplitImageRects( CvRect );
	bool isConsHasBeenSelectd( int which );
	void removeConsSelectd( int which );
	int clickFind(Point point);	// return -1 is invalid
	void concourSelect( cv::Point cvPoint );
	void doMultClipRectSelect( cv::Point start , cv::Point end );
	void doMultConsSelect( cv::Point start , cv::Point end );
	void deleteContous();
	void picturePointRefectToRoiPoint( cv::Point in , cv::Point &out  );
	void saveContoursAndFile(vector<vector<Point>> contours, cv::Mat *src, const char *filename);
	cv::Mat * readContoursAndFile(vector<vector<Point>> *contours, const char *filename);
	void outputLayoutFile();
	void contourAlphaMask( vector<Point> contour );
	void XlmGenerate(  String outFile,vector<RECT_VECTOR>  RelateList  );
	void printOutRelativeListLayout( MSXML2::IXMLDOMDocumentPtr pDoc , MSXML2::IXMLDOMElementPtr xmlRoot , vector<RECT_VECTOR>  RelateList );
	void printOutRelativeLayout(  MSXML2::IXMLDOMDocumentPtr pDoc ,MSXML2::IXMLDOMElementPtr pElement , RECT_VECTOR  *Relative );
	void consAdjust( int x , int y );

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
	void generateFilterMat();//����͸���Ⱦ���
	afx_msg void OnBnCutOut();
	afx_msg void OnDblclkShowPicture();
	afx_msg void OnBnClickedRangeSelect();
	CButton m_rangeSelectbtn;

	//ȡ����inδ����ĵ�λ��������ͨ��ľ�������
	CvRect GetConnectAreaRect( cv::Mat &mat  , Point in );

	
	vector<Point>GetConnectAreaPoint(cv::Mat &mat  , Point in );

	//��mat�����пɼ�����ָ��n��ͼ��
	vector<CvRect> SpliteVisiableImage( cv::Mat mat );

	afx_msg void OnBnClickedBiaoding();
	afx_msg void OnBnClickedSaveCalResult();
	afx_msg void OnBnClickedShowBiaodingResult();
	afx_msg void OnBnClickedMergeArea();
	afx_msg void OnBnClickedMeger();
	afx_msg void OnBnClickedFix();
	afx_msg void OnBnClickedSaveLayoutFile();


};
