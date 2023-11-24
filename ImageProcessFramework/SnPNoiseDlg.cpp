// SnPNoiseDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "SnPNoiseDlg.h"
#include "Noise.h"


// SnPNoiseDlg 대화 상자

IMPLEMENT_DYNAMIC(SnPNoiseDlg, CDialog)

SnPNoiseDlg::SnPNoiseDlg(float* s_prob, float* p_prob, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG9, pParent), s_prob(s_prob), p_prob(p_prob)
{

}

SnPNoiseDlg::~SnPNoiseDlg()
{
}

void SnPNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, sp_str);
	DDX_Control(pDX, IDC_EDIT7, pp_str);
}


BEGIN_MESSAGE_MAP(SnPNoiseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &SnPNoiseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SnPNoiseDlg 메시지 처리기


void SnPNoiseDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString salt_prop_str, pepper_prop_str;
	sp_str.GetWindowTextA(salt_prop_str);
	pp_str.GetWindowTextA(pepper_prop_str);
	*s_prob = atof(std::string(salt_prop_str).c_str());
	*p_prob = atof(std::string(pepper_prop_str).c_str());
	this->EndDialog(0);
}
