#pragma once
#include "afxdialogex.h"

// ResizeDlg 대화 상자

class ResizeDlg : public CDialog
{
	DECLARE_DYNAMIC(ResizeDlg)

public:
	ResizeDlg(int* resize_type, double* resize_scale, CWnd* pParent = nullptr);// 표준 생성자입니다.
	virtual ~ResizeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	int* resize_type;
	double* resize_scale;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int radio_button_opt = 0;
	CWnd scale;
	afx_msg void RadioCtrl(UINT id);
	afx_msg void OnBnClickedButton1();
};
