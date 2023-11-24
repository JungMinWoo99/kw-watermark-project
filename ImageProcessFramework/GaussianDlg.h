#pragma once
#include "afxdialogex.h"

// GaussianDlg 대화 상자

class GaussianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GaussianDlg)

public:
	GaussianDlg(int* gau_size, double* gau_sigma, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~GaussianDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	int* gau_size;
	double* gau_sigma;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CWnd sigma,size;
	afx_msg void OnBnClickedButton1();
};
