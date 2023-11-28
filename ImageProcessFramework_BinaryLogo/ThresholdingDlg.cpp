// ThresholdingDlg.cpp: 구현 파일
//


#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "ThresholdingDlg.h"
#include <string>

// ThresholdingDlg 대화 상자

IMPLEMENT_DYNAMIC(ThresholdingDlg, CDialog)

ThresholdingDlg::ThresholdingDlg(int* t1, int* t2, CWnd* pParent)
	: CDialog(IDD_DIALOG10, pParent), t1(t1), t2(t2)
{

}

ThresholdingDlg::~ThresholdingDlg()
{
}

void ThresholdingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, t1_str);
	DDX_Control(pDX, IDC_EDIT7, t2_str);
}


BEGIN_MESSAGE_MAP(ThresholdingDlg, CDialog)
	ON_BN_CLICKED(IDOK2, &ThresholdingDlg::OnBnClickedOk2)
END_MESSAGE_MAP()


// ThresholdingDlg 메시지 처리기


void ThresholdingDlg::OnBnClickedOk2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString t1_prop_str, t2_prop_str;
	t1_str.GetWindowTextA(t1_prop_str);
	t2_str.GetWindowTextA(t2_prop_str);
	*t1 = atoi(std::string(t1_prop_str).c_str());
	*t2 = atoi(std::string(t2_prop_str).c_str());
	this->EndDialog(0);
}
