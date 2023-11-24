// PLCSDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "PLCSDlg.h"


// PLCSDlg 대화 상자

IMPLEMENT_DYNAMIC(PLCSDlg, CDialogEx)

PLCSDlg::PLCSDlg(cv::Point* point, CWnd* pParent)
	: CDialogEx(IDD_DIALOG7, pParent), point(point)
{

}

PLCSDlg::~PLCSDlg()
{
}

void PLCSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, x1);
	DDX_Control(pDX, IDC_EDIT8, x2);
	DDX_Control(pDX, IDC_EDIT7, y1);
	DDX_Control(pDX, IDC_EDIT9, y2);
}


BEGIN_MESSAGE_MAP(PLCSDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &PLCSDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// PLCSDlg 메시지 처리기


void PLCSDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString x1_str,x2_str,y1_str,y2_str;
	x1.GetWindowTextA(x1_str);
	x2.GetWindowTextA(x2_str);
	y1.GetWindowTextA(y1_str);
	y2.GetWindowTextA(y2_str);

	point[0].x=atoi(std::string(x1_str).c_str());
	point[1].x = atoi(std::string(x2_str).c_str());
	point[0].y = atoi(std::string(y1_str).c_str());
	point[1].y = atoi(std::string(y2_str).c_str());

	this->EndDialog(0);
}
