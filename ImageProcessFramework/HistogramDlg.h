#pragma once
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>

// HistogramDlg 대화 상자

class HistogramDlg : public CDialog
{
	DECLARE_DYNAMIC(HistogramDlg)

public:
	HistogramDlg(cv::Point* point, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~HistogramDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	cv::Point* point;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CWnd x1,x2,x3,y1,y2,y3;
	afx_msg void OnBnClickedButton1();
};
