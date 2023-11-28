// ResizeProcessDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "ResizeProcessDlg.h"

#include <string>


// ResizeProcessDlg 대화 상자

IMPLEMENT_DYNAMIC(ResizeProcessDlg, CDialog)

ResizeProcessDlg::ResizeProcessDlg(int* resize_process_type, int* resize_process_width, int* resize_process_height, CWnd* pParent)
	: CDialog(IDD_DIALOG1, pParent), resize_process_type(resize_process_type), resize_process_width(resize_process_width), resize_process_height(resize_process_height)
{

}

ResizeProcessDlg::~ResizeProcessDlg()
{
}

void ResizeProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, radio_button_opt);
	DDX_Control(pDX, IDC_RESIZE_WIDTH, width);
	DDX_Control(pDX, IDC_RESIZE_HEIGHT, height);
}


BEGIN_MESSAGE_MAP(ResizeProcessDlg, CDialog)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, RadioCtrl)
	ON_BN_CLICKED(IDC_BUTTON1, &ResizeProcessDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// ResizeProcessDlg 메시지 처리기

void ResizeProcessDlg::RadioCtrl(UINT id)
{
	switch (id)
	{
	case IDC_RADIO1:
		radio_button_opt = 0;
		break;
	case IDC_RADIO2:
		radio_button_opt = 1;
		break;
	}
}
void ResizeProcessDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString width_str = _T("");
	CString height_str = _T("");
	width.GetWindowTextA(width_str);
	height.GetWindowTextA(height_str);
	*resize_process_type = radio_button_opt;
	*resize_process_width = atoi(std::string(width_str).c_str());
	*resize_process_height = atoi(std::string(height_str).c_str());
	this->EndDialog(0);
}
