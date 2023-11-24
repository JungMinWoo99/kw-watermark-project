#pragma once
#include "afxdialogex.h"

// AvgFilter 대화 상자

class AvgFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(AvgFilterDlg)

public:
	AvgFilterDlg(int* avg_filter_size, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AvgFilterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	int* avg_filter_size;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CWnd size;
	afx_msg void OnBnClickedButton1();
};
