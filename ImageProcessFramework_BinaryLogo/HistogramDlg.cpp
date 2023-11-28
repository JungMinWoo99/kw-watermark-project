// HistogramDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProcessFramework.h"
#include "afxdialogex.h"
#include "HistogramDlg.h"


// HistogramDlg 대화 상자

IMPLEMENT_DYNAMIC(HistogramDlg, CDialog)

HistogramDlg::HistogramDlg(cv::Point* point, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG4, pParent), point(point)
{

}

HistogramDlg::~HistogramDlg()
{
}

void HistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT3, x1);
	DDX_Control(pDX, IDC_EDIT1, x2);
	DDX_Control(pDX, IDC_EDIT5, x3);
	DDX_Control(pDX, IDC_EDIT4, y1);
	DDX_Control(pDX, IDC_EDIT2, y2);
	DDX_Control(pDX, IDC_EDIT6, y3);

}


BEGIN_MESSAGE_MAP(HistogramDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &HistogramDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// HistogramDlg 메시지 처리기


void HistogramDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str = _T("");
	x1.GetWindowTextA(str);
	point[0].x = atoi(std::string(str).c_str());
	x2.GetWindowTextA(str);
	point[1].x = atoi(std::string(str).c_str());
	x3.GetWindowTextA(str);
	point[2].x = atoi(std::string(str).c_str());
	y1.GetWindowTextA(str);
	point[0].y = atoi(std::string(str).c_str());
	y2.GetWindowTextA(str);
	point[1].y = atoi(std::string(str).c_str());
	y3.GetWindowTextA(str);
	point[2].y = atoi(std::string(str).c_str());

	this->EndDialog(0);
}
