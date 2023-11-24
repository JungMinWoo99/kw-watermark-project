// AvgFilter.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "AvgFilter.h"
#include <string>

// AvgFilter 대화 상자

IMPLEMENT_DYNAMIC(AvgFilterDlg, CDialog)

AvgFilterDlg::AvgFilterDlg(int* avg_filter_size, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG5, pParent), avg_filter_size(avg_filter_size)
{

}

AvgFilterDlg::~AvgFilterDlg()
{
}

void AvgFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, size);
}


BEGIN_MESSAGE_MAP(AvgFilterDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &AvgFilterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// AvgFilter 메시지 처리기


void AvgFilterDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString size_str = _T("");
	size.GetWindowTextA(size_str);
	*avg_filter_size = atoi(std::string(size_str).c_str());
	this->EndDialog(0);
}
