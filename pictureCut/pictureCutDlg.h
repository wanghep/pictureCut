
// pictureCutDlg.h : ͷ�ļ�
//

#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"  

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
	cv::Mat showImageMat; // ���������ʾ��mat ����srcMat�ϵ����Զ���ָʾ
	CvRect  roiRect ; // ������ʾ����� 

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
	void showMatImgToWnd(CWnd* pWnd, const cv::Mat& img);
public:
	afx_msg void OnBnClickedZoomUp();
	afx_msg void OnBnClickedZoomDown();
	afx_msg void OnThemechangedShowPicture(NMHDR *pNMHDR, LRESULT *pResult);
};
