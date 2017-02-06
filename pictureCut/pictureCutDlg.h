
// pictureCutDlg.h : ͷ�ļ�
//

#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"  
#include "algorithm\PictureCutAlgorithm.h"

#pragma once
using namespace cv;

// CpictureCutDlg �Ի���

class CpictureCutDlg : public CDialogEx
{
// ����
public:
	CpictureCutDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PICTURECUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
private:
	cv::Mat srcMat;
	cv::Mat subSrcImageMat; // ���Ż�ָ��rect��ͼ��

	cv::Mat showImageMat; // ���������ʾ��mat ����srcMat�ϵ����Զ���ָʾ

	CvRect  roiRect ; // ������ʾ����� 
	vector<vector<Point> >  contours ; // srcMat�ϵ���ͨ�� 

	PictureCutAlgorithm Pca;

	BOOL showSourcePicture;
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
	void Update( Point *pt1 = NULL, Point *pt2 = NULL );
	void showMatImgToWnd( CWnd* pWnd, cv::Mat img );
public:
	afx_msg void OnBnClickedZoomUp();
	afx_msg void OnBnClickedZoomDown();
	afx_msg void OnThemechangedShowPicture(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_showPicture;
	afx_msg void OnClickedShowPicture();
	afx_msg void OnBnClickedCut();

	afx_msg void OnBnShowSourcePicture();
};
