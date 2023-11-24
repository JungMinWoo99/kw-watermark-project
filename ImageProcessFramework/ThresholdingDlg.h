#pragma once
#include "afxdialogex.h"


// ThresholdingDlg 대화 상자

class ThresholdingDlg : public CDialog
{
	DECLARE_DYNAMIC(ThresholdingDlg)

public:
	ThresholdingDlg(int* t1, int* t2, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ThresholdingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	CWnd t1_str, t2_str;
	int* t1;
	int* t2;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk2();
};
