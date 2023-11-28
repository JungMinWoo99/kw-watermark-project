// ResizeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "ResizeDlg.h"
#include <string>


// ResizeDlg 대화 상자

IMPLEMENT_DYNAMIC(ResizeDlg, CDialog)


ResizeDlg::ResizeDlg(int* resize_type, double* resize_scale, CWnd* pParent)
	: CDialog(IDD_DIALOG2, pParent), resize_type(resize_type), resize_scale(resize_scale)
{

}

ResizeDlg::~ResizeDlg()
{
}

void ResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_MODE_1, radio_button_opt);
	DDX_Control(pDX, IDC_EDIT1, scale);
}


BEGIN_MESSAGE_MAP(ResizeDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_MODE_1, IDC_MODE_3, RadioCtrl)
	ON_BN_CLICKED(IDC_BUTTON1, &ResizeDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// ResizeDlg 메시지 처리기

afx_msg void ResizeDlg::RadioCtrl(UINT id)
{
	switch (id)
	{
	case IDC_MODE_1:
		radio_button_opt = 0;
		break;
	case IDC_MODE_2:
		radio_button_opt = 1;
		break;
	case IDC_MODE_3:
		radio_button_opt = 2;
		break;
	}
}

void ResizeDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString scale_str = _T("");
	scale.GetWindowTextA(scale_str);
	*resize_scale = atof(std::string(scale_str).c_str());
	*resize_type = radio_button_opt;
	this->EndDialog(0);
}
