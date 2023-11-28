#pragma once
#include "afxdialogex.h"


// ResizeProcessDlg 대화 상자

class ResizeProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(ResizeProcessDlg)

public:
	ResizeProcessDlg(int* resize_process_type, int* resize_process_width, int* resize_process_height, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ResizeProcessDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	int* resize_process_type;
	int* resize_process_width;
	int* resize_process_height;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	int radio_button_opt = 0;
	CWnd width;
	CWnd height;
	afx_msg void RadioCtrl(UINT id);
	afx_msg void OnBnClickedButton1();
};
