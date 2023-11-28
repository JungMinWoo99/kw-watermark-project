#pragma once
#include "afxdialogex.h"


// SnPNoiseDlg 대화 상자

class SnPNoiseDlg : public CDialog
{
	DECLARE_DYNAMIC(SnPNoiseDlg)

public:
	SnPNoiseDlg(float* s_prob, float* p_prob, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SnPNoiseDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	CWnd sp_str, pp_str;
	float* s_prob;
	float* p_prob;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
