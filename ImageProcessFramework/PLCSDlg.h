#pragma once
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>

// PLCSDlg 대화 상자

class PLCSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PLCSDlg)

public:
	PLCSDlg(cv::Point* point, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~PLCSDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	cv::Point* point;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CWnd x1,x2,y1,y2;
};
