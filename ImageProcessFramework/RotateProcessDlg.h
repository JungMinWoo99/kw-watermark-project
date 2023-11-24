#pragma once
#include "afxdialogex.h"

// RotateProcessDlg 대화 상자

class RotateProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(RotateProcessDlg)

public:
	RotateProcessDlg(int* rotate_process_type,int* rotate_process_angle, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~RotateProcessDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	int* rotate_process_type;
	int* rotate_process_angle;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int radio_button_opt = 0;
	CWnd angle;

	afx_msg void RadioCtrl(UINT id);
	afx_msg void OnBnClickedButton1();
};
