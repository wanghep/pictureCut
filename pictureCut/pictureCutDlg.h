
// pictureCutDlg.h : 头文件
//

#include<opencv2\opencv.hpp>
#include "cv.h"     
#include "highgui.h"  

#pragma once
using namespace cv;

// CpictureCutDlg 对话框
class CpictureCutDlg : public CDialogEx
{
// 构造
public:
	CpictureCutDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PICTURECUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
private:
	cv::Mat srcMat;
	cv::Mat showImageMat; // 用于输出显示的mat ，在srcMat上叠加自定义指示
	CvRect  roiRect ; // 限制显示区域的 

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
	void showMatImgToWnd(CWnd* pWnd, const cv::Mat& img);
public:
	afx_msg void OnBnClickedZoomUp();
	afx_msg void OnBnClickedZoomDown();
	afx_msg void OnThemechangedShowPicture(NMHDR *pNMHDR, LRESULT *pResult);
};
