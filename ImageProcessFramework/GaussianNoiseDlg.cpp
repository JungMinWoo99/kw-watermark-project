// GaussianNoiseDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "GaussianNoiseDlg.h"
#include <string>


// GaussianNoiseDlg 대화 상자

IMPLEMENT_DYNAMIC(GaussianNoiseDlg, CDialog)

GaussianNoiseDlg::GaussianNoiseDlg(float* mean, float* std_dev,CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG8, pParent), mean(mean), std_dev(std_dev)
{

}

GaussianNoiseDlg::~GaussianNoiseDlg()
{
}

void GaussianNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_str);
	DDX_Control(pDX, IDC_EDIT7, sd_str);
}


BEGIN_MESSAGE_MAP(GaussianNoiseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &GaussianNoiseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// GaussianNoiseDlg 메시지 처리기


void GaussianNoiseDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString mean_str, std_dev_str;
	m_str.GetWindowTextA(mean_str);
	sd_str.GetWindowTextA(std_dev_str);
	*mean = atof(std::string(mean_str).c_str());
	*std_dev = atof(std::string(std_dev_str).c_str());
	this->EndDialog(0);
}
