// GaussianDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "GaussianDlg.h"
#include <string>

// GaussianDlg 대화 상자

IMPLEMENT_DYNAMIC(GaussianDlg, CDialogEx)

GaussianDlg::GaussianDlg(int* gau_size, double* gau_sigma, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent), gau_size(gau_size), gau_sigma(gau_sigma)
{

}

GaussianDlg::~GaussianDlg()
{
}

void GaussianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT7, sigma);
	DDX_Control(pDX, IDC_EDIT1, size);
}


BEGIN_MESSAGE_MAP(GaussianDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &GaussianDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// GaussianDlg 메시지 처리기


void GaussianDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString size_str = _T("");
	CString sigma_str = _T("");
	sigma.GetWindowTextA(sigma_str);
	size.GetWindowTextA(size_str);
	*gau_size = atoi(std::string(size_str).c_str());
	*gau_sigma = atof(std::string(sigma_str).c_str());
	this->EndDialog(0);
}
