// RotateProcessDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "RotateProcessDlg.h"
#include <string>

// RotateProcessDlg 대화 상자

IMPLEMENT_DYNAMIC(RotateProcessDlg, CDialog)

RotateProcessDlg::RotateProcessDlg(int* rotate_process_type, int* rotate_process_angle, CWnd* pParent)
	: CDialog(IDD_DIALOG3, pParent), rotate_process_type(rotate_process_type), rotate_process_angle(rotate_process_angle)
{

}

RotateProcessDlg::~RotateProcessDlg()
{
}

void RotateProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ROTATE_RADIO1, radio_button_opt);
	DDX_Control(pDX, IDC_ROTATE_ANGLE, angle);
}


BEGIN_MESSAGE_MAP(RotateProcessDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_ROTATE_RADIO1, IDC_ROTATE_RADIO2, RadioCtrl)
	ON_BN_CLICKED(IDC_BUTTON1, &RotateProcessDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// RotateProcessDlg 메시지 처리기

void RotateProcessDlg::RadioCtrl(UINT id)
{
	switch (id)
	{
	case IDC_ROTATE_RADIO1:
		radio_button_opt = 0;
		break;
	case IDC_ROTATE_RADIO2:
		radio_button_opt = 1;
		break;
	}
}

void RotateProcessDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString angle_str = _T("");
	angle.GetWindowTextA(angle_str);
	*rotate_process_type = radio_button_opt;
	*rotate_process_angle = atoi(std::string(angle_str).c_str());
	this->EndDialog(0);
}
