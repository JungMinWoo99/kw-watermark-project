#pragma once
#include "afxdialogex.h"


// GaussianNoiseDlg 대화 상자

class GaussianNoiseDlg : public CDialog
{
	DECLARE_DYNAMIC(GaussianNoiseDlg)

public:
	GaussianNoiseDlg(float* mean, float* std_dev, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~GaussianNoiseDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	CWnd m_str, sd_str;
	float* mean;
	float* std_dev;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
