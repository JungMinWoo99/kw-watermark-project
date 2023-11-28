
// ImageProcessFrameworkDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ImageProcessFramework.h"
#include "ImageProcessFrameworkDlg.h"
#include "afxdialogex.h"
#include "ResizeDlg.h"
#include "ResizeProcessDlg.h"
#include "RotateProcessDlg.h"
#include "HistogramDlg.h"
#include "AvgFilter.h"
#include "GaussianDlg.h"
#include "PLCSDlg.h"
#include "GaussianNoiseDlg.h"
#include "SnPNoiseDlg.h"
#include "ThresholdingDlg.h"

#include "SimpleImageProcessor.h"
#include "BicubicInterpolation.h"
#include "BilinearInterpolation.h"
#include "Filter.h"
#include "HistogramProcessor.h"
#include "PSNR.h"
#include "Noise.h"
#include "Watermark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageProcessFrameworkDlg 대화 상자



CImageProcessFrameworkDlg::CImageProcessFrameworkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGEPROCESSFRAMEWORK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageProcessFrameworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageProcessFrameworkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32771, &CImageProcessFrameworkDlg::OpenImageFile)
	ON_COMMAND(ID_32775, &CImageProcessFrameworkDlg::ViewScaled)
	ON_COMMAND(ID_32778, &CImageProcessFrameworkDlg::SaveAsGray)
	ON_COMMAND(ID_32782, &CImageProcessFrameworkDlg::ViewGray)
	ON_COMMAND(ID_32777, &CImageProcessFrameworkDlg::SaveASYUV444)
	ON_COMMAND(ID_32781, &CImageProcessFrameworkDlg::ViewYUV444)
	ON_COMMAND(ID_32779, &CImageProcessFrameworkDlg::SaveAsRGBProgressive)
	ON_COMMAND(ID_32783, &CImageProcessFrameworkDlg::ViewRGBProgressive)
	ON_COMMAND(ID_32786, &CImageProcessFrameworkDlg::ResizeProcess)
	ON_COMMAND(ID_32785, &CImageProcessFrameworkDlg::Undo)
	ON_COMMAND(ID_32787, &CImageProcessFrameworkDlg::RotateProcess)
	ON_COMMAND(ID_32789, &CImageProcessFrameworkDlg::MatchHitogram)
	ON_COMMAND(ID_32788, &CImageProcessFrameworkDlg::AvgFilter)
	ON_COMMAND(ID_32790, &CImageProcessFrameworkDlg::GaussianFilter)
	ON_COMMAND(ID_32791, &CImageProcessFrameworkDlg::ShowHistogram)
	ON_COMMAND(ID_32793, &CImageProcessFrameworkDlg::LinearContrastStretching)
	ON_COMMAND(ID_32795, &CImageProcessFrameworkDlg::AddGaussianNoise)
	ON_COMMAND(ID_32796, &CImageProcessFrameworkDlg::AddSnPNoise)
	ON_COMMAND(ID_32797, &CImageProcessFrameworkDlg::GeometricMeanFilter)
	ON_COMMAND(ID_32798, &CImageProcessFrameworkDlg::MedianFilter)
	ON_COMMAND(ID_32799, &CImageProcessFrameworkDlg::AdaptiveMedianFilter)
	ON_COMMAND(ID_PSNR_PSNR, &CImageProcessFrameworkDlg::ViewPSNR)
	ON_COMMAND(ID_32801, &CImageProcessFrameworkDlg::Thresholding)
	ON_COMMAND(ID_32802, &CImageProcessFrameworkDlg::InsertWM)
	ON_COMMAND(ID_32803, &CImageProcessFrameworkDlg::ExtractWM)
	ON_COMMAND(ID_JPEG32804, &CImageProcessFrameworkDlg::JPEGTest)
END_MESSAGE_MAP()


// CImageProcessFrameworkDlg 메시지 처리기

BOOL CImageProcessFrameworkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CImageProcessFrameworkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImageProcessFrameworkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImageProcessFrameworkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//사용자 정의 함수
void CImageProcessFrameworkDlg::PrintScreen()
{
	screen.Destroy();
	auto current_raw_data = raw_data_manager.GetCurrentData().get();

	uchar* pixel_data_buf = current_raw_data->raw_data;
	int bpp = current_raw_data->byte_per_pixel;
	int fliped_data_width = current_raw_data->width;
	int fliped_data_hight = current_raw_data->height;
	screen.Create(fliped_data_width, fliped_data_hight, bpp * 8);

	uchar* dstData = (uchar*)screen.GetBits();
	int dstPitch = screen.GetPitch();
	for (int i = 0; i < fliped_data_hight; ++i)
	{
		memcpy(dstData, pixel_data_buf, fliped_data_width * bpp);
		dstData += dstPitch;
		pixel_data_buf += fliped_data_width * bpp;
	}
	CRect rect;
	int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
	int menuBarHeight = GetSystemMetrics(SM_CYMENUSIZE);
	GetWindowRect(&rect);
	MoveWindow(rect.left, rect.top, screen.GetWidth() + 16, screen.GetHeight() + 17 + titleBarHeight + menuBarHeight);

	CClientDC dc(this);
	screen.Draw(dc, 0, 0);
}

void CImageProcessFrameworkDlg::PrintView(std::shared_ptr<ImgRawData> print_img)
{
	screen.Destroy();

	cv::Mat view = RawDataConvert::RawDataToMat(print_img);

	if (print_img->color_space == ImgRawData::Gray)
		cv::imshow("gray view", view);
	else if (print_img->color_space == ImgRawData::RGB_progressive)
		cv::imshow("rgb_p view", view);
	else if (print_img->color_space == ImgRawData::YUV444)
		cv::imshow("yuv view", view);
}

void CImageProcessFrameworkDlg::SaveRawData(std::string save_file, uchar* save_data, size_t data_size)
{
	FILE* fp_save;
	errno_t error = fopen_s(&fp_save, save_file.c_str(), "wb");
	if (error == 0) {
		fwrite((void*)save_data, sizeof(uchar), data_size, fp_save);
		fclose(fp_save);
	}
	else
	{
		if (error == ENOENT)
			MessageBox(_T("파일이 존재하지 않습니다."));
		else if (error == EACCES)
			MessageBox(_T("파일에 접근할 권한이 없습니다."));
		else
			MessageBox(_T("파일 열기 실패"));
	}
}

void CImageProcessFrameworkDlg::OpenImageFile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG, *.PNG) | *.BMP;*.GIF;*.JPG;*PNG;*.bmp;*.jpg;*.gif;*.png;|");

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		CT2CA pszConvertedAnsiString(pathName);
		cv::Mat frame = cv::imread(std::string(pszConvertedAnsiString));   //frame은 Mat 객체
		if (frame.data)
		{
			raw_data_manager.InputNewSrc(frame);
			PrintScreen();
		}
		else
		{
			MessageBox(_T("Image Open Error"));
			return;
		}
	}
}

void CImageProcessFrameworkDlg::Undo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	raw_data_manager.UndoProcess();
	PrintScreen();
}

void CImageProcessFrameworkDlg::ViewScaled()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	int resize_type = 0;
	double resize_scale = 1;

	ResizeDlg resize_dlg(&resize_type, &resize_scale);
	resize_dlg.DoModal();
	cv::Mat view = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat scaled;
	cv::resize(view, scaled, cv::Size(view.cols * resize_scale, view.rows * resize_scale));

	auto print_img = RawDataConvert::MatToRawData(scaled, ImgRawData::BGR);

	if (resize_type == 0)
		print_img = RawDataConvert::BGRToGray(print_img);
	else if (resize_type == 1)
		print_img = RawDataConvert::BGRToYUV444(print_img);
	else if (resize_type == 2)
		print_img = RawDataConvert::BGRToRGBProgressive(print_img);

	view = RawDataConvert::RawDataToMat(print_img);
	cv::imshow("view", view);
}

void CImageProcessFrameworkDlg::ViewGray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	auto view_data = RawDataConvert::BGRToGray(raw_data_manager.GetCurrentData());
	CImageProcessFrameworkDlg::PrintView(view_data);
}

void CImageProcessFrameworkDlg::ViewYUV444()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	auto view_data = RawDataConvert::BGRToYUV444(raw_data_manager.GetCurrentData());
	CImageProcessFrameworkDlg::PrintView(view_data);
}

void CImageProcessFrameworkDlg::ViewRGBProgressive()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	auto view_data = RawDataConvert::BGRToRGBProgressive(raw_data_manager.GetCurrentData());

	CImageProcessFrameworkDlg::PrintView(view_data);
}

void CImageProcessFrameworkDlg::SaveAsGray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	auto save_data = RawDataConvert::BGRToGray(raw_data_manager.GetCurrentData());
	size_t data_size = save_data->height * save_data->width * save_data->byte_per_pixel;
	static TCHAR BASED_CODE szFilter[] = _T("Raw Data 파일(*.YUV, *.RAW, *.IMG, *.RGB, *.GRAY, *.yuv, *.raw, *.img, *.rgb, *.gray) | *.YUV; *.RAW; *.IMG; *.RGB; *.GRAY; *.yuv; *.raw; *.img; *.rgb; *.gray; |");

	CFileDialog dlg(FALSE, _T("*.gray"), _T("img.gray"), OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlg.m_ofn.lpstrDefExt = nullptr;
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		CT2CA pszConvertedAnsiString(pathName);
		SaveRawData(std::string(pszConvertedAnsiString), save_data->raw_data, data_size);
	}
}

void CImageProcessFrameworkDlg::SaveASYUV444()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	auto save_data = RawDataConvert::BGRToYUV444(raw_data_manager.GetCurrentData());
	size_t data_size = save_data->height * save_data->width * save_data->byte_per_pixel;
	static TCHAR BASED_CODE szFilter[] = _T("Raw Data 파일(*.YUV, *.RAW, *.IMG, *.RGB, *.GRAY, *.yuv, *.raw, *.img, *.rgb, *.gray) | *.YUV; *.RAW; *.IMG; *.RGB; *.GRAY; *.yuv; *.raw; *.img; *.rgb; *.gray; |");

	CFileDialog dlg(FALSE, _T("*.yuv"), _T("img.yuv"), OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlg.m_ofn.lpstrDefExt = nullptr;
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		CT2CA pszConvertedAnsiString(pathName);
		SaveRawData(std::string(pszConvertedAnsiString), save_data->raw_data, data_size);
	}
}

void CImageProcessFrameworkDlg::SaveAsRGBProgressive()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	auto save_data = RawDataConvert::BGRToRGBProgressive(raw_data_manager.GetCurrentData());
	size_t data_size = save_data->height * save_data->width * save_data->byte_per_pixel;
	static TCHAR BASED_CODE szFilter[] = _T("Raw Data 파일(*.YUV, *.RAW, *.IMG, *.RGB, *.GRAY, *.yuv, *.raw, *.img, *.rgb, *.gray) | *.YUV; *.RAW; *.IMG; *.RGB; *.GRAY; *.yuv; *.raw; *.img; *.rgb; *.gray; |");

	CFileDialog dlg(FALSE, _T("*.rgb"), _T("img.rgb"), OFN_CREATEPROMPT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlg.m_ofn.lpstrDefExt = nullptr;
	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		CT2CA pszConvertedAnsiString(pathName);
		SaveRawData(std::string(pszConvertedAnsiString), save_data->raw_data, data_size);
	}
}

void CImageProcessFrameworkDlg::ResizeProcess()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	int resize_process_type = 0;
	int resize_process_width = 100;
	int resize_process_height = 100;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	ResizeProcessDlg rp_dlg(&resize_process_type,&resize_process_width,&resize_process_height);
	rp_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());

	ImageMaster processer;
	if (resize_process_type == 0)
		current_img = processer.ResizeImage(current_img, resize_process_width, resize_process_height, BilinearInterpolation());
	else if (resize_process_type == 1)
		current_img = processer.ResizeImage(current_img, resize_process_width, resize_process_height, BicubicInterpolation());

	auto current_raw_data = RawDataConvert::MatToRawData(current_img, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::RotateProcess()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	int rotate_process_type = 0;
	int rotate_process_angle = 0;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	RotateProcessDlg rt_dlg(&rotate_process_type,&rotate_process_angle);
	rt_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());

	ImageMaster processer;
	if (rotate_process_type == 0)
		current_img = processer.RotateImage(current_img, rotate_process_angle, BilinearInterpolation());
	else if (rotate_process_type == 1)
		current_img = processer.RotateImage(current_img, rotate_process_angle, BicubicInterpolation());

	auto current_raw_data = RawDataConvert::MatToRawData(current_img, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::MatchHitogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cv::Point point[3];

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	HistogramDlg h_dlg(point);
	h_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat output;
	HistogramProcessor p;
	p.InputImg(current_img);
	p.MatchPiesewiselinearHistogram(point[0], point[1], point[2]);
	p.GetResult(output);

	auto current_raw_data = RawDataConvert::MatToRawData(output, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::ShowHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	HistogramProcessor p;
	p.InputImg(current_img);
	for (int i = 0; i < current_img.elemSize(); i++)
		p.ShowHistogram(i);
}

void CImageProcessFrameworkDlg::GaussianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	int gau_size;
	double gau_sigma;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	auto view_mat = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	GaussianDlg g_dlg(&gau_size, &gau_sigma);
	g_dlg.DoModal();

	cv::GaussianBlur(view_mat, view_mat, cv::Size(gau_size, gau_size), gau_sigma);
	cv::imshow("GaussianBlur", view_mat);
}

void CImageProcessFrameworkDlg::LinearContrastStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cv::Point point[2];
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	ImageMaster processer;
	PLCSDlg dlg(point);
	dlg.DoModal();
	cv::Mat output = processer.LinearContrastStretching(current_img, point[0], point[1]);
	auto current_raw_data = RawDataConvert::MatToRawData(output, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::AddGaussianNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	float m,s;
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	GaussianNoiseDlg dlg(&m, &s);
	dlg.DoModal();

	GaussianNoise g(m,s);
	
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());

	g.AddNoise(current_img);
	auto current_raw_data = RawDataConvert::MatToRawData(current_img, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::AddSnPNoise()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	float s, p;
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}
	SnPNoiseDlg dlg(&s, &p);
	dlg.DoModal();

	SnPNoise snp(s, p);

	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());

	snp.AddNoise(current_img);
	auto current_raw_data = RawDataConvert::MatToRawData(current_img, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::AvgFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int avg_filter_size;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	AvgFilterDlg a_dlg(&avg_filter_size);
	a_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat output;

	Filter filter;
	int size = avg_filter_size;
	double** mv_filter = new double* [size];
	for (int i = 0; i < size; i++)
		mv_filter[i] = new double[size];
	for (int i = 0; i < size; i++)
	{
		for (int l = 0; l < size; l++)
			mv_filter[i][l] = 1.0 / (size * size);
	}

	filter.InputFilter(mv_filter, size);
	filter.InputImg(current_img);
	filter.FilterImg();

	filter.GetFilterReslut(output);

	auto current_raw_data = RawDataConvert::MatToRawData(output, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
	delete[] mv_filter;
}

void CImageProcessFrameworkDlg::GeometricMeanFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int filter_size;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	AvgFilterDlg a_dlg(&filter_size);
	a_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat output;

	Filter filter;
	filter.InputImg(current_img);
	filter.GeometricMeanFilter(filter_size);

	filter.GetFilterReslut(output);

	auto current_raw_data = RawDataConvert::MatToRawData(output, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::MedianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int filter_size;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	AvgFilterDlg a_dlg(&filter_size);
	a_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat output;

	Filter filter;
	filter.InputImg(current_img);
	filter.MedianFilter(filter_size);

	filter.GetFilterReslut(output);

	auto current_raw_data = RawDataConvert::MatToRawData(output, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::AdaptiveMedianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int filter_size;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	AvgFilterDlg a_dlg(&filter_size);
	a_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat output;

	Filter filter;
	filter.InputImg(current_img);
	filter.AdaptiveMedianFilter(filter_size);

	filter.GetFilterReslut(output);

	auto current_raw_data = RawDataConvert::MatToRawData(output, ImgRawData::BGR);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::Thresholding()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int t1 = 128,t2 = 128;

	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	ThresholdingDlg t_dlg(&t1,&t2);
	t_dlg.DoModal();
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat output;

	Filter filter;
	filter.Threshold(current_img, output,t1,t2);

	cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
	auto current_raw_data = RawDataConvert::MatToRawData(output, ImgRawData::Gray);

	raw_data_manager.PushRawData(current_raw_data);

	PrintScreen();
}

void CImageProcessFrameworkDlg::ViewPSNR()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	static int cnt = 0;
	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat src_img = RawDataConvert::RawDataToMat(raw_data_manager.GetSrcData());

	PSNR psnr;
	double psnr_value = psnr.GetPSNR(src_img, current_img);
	cv::imshow(cv::format("res%d, PSNR:%f", ++cnt,psnr_value), current_img);
}

void CImageProcessFrameworkDlg::InsertWM()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	cv::Mat src_img = RawDataConvert::RawDataToMat(raw_data_manager.GetSrcData());

	// 이미지를 YUV로 변환합니다.
	cv::Mat yuvImage;
	cv::cvtColor(src_img, yuvImage, cv::COLOR_BGR2YUV);

	// Y 성분만 추출합니다.
	std::vector<cv::Mat> channels;
	cv::split(yuvImage, channels);
	cv::Mat yChannel = channels[0];

	//Y부분만 DCT변환
	BlockMatrix dct_data;
	DctTransformer::TransformImgToDct(yChannel, dct_data);

	//워터 마크 생성
	randomSequence = generateRandomSequence();

	//워터마크 삽입
	WatermarkInsert(dct_data, randomSequence);

	//워터마크 삽입한 이미지 생성
	cv::Mat watermarked;
	DctTransformer::TransformDctToImg(dct_data, watermarked);
	channels[0] = watermarked;

	cv::Mat res;
	cv::merge(channels, res);
	cv::cvtColor(res, res, cv::COLOR_YUV2BGR);

	PSNR psnr;
	double psnr_value = psnr.GetPSNR(src_img, res);
	cv::imshow(cv::format("Watermarked Img, PSNR:%f", psnr_value), res);

	auto current_raw_data = RawDataConvert::MatToRawData(res, ImgRawData::BGR);
	raw_data_manager.PushRawData(current_raw_data);
	PrintScreen();
}


void CImageProcessFrameworkDlg::ExtractWM()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat src_img = RawDataConvert::RawDataToMat(raw_data_manager.GetSrcData());

	cv::cvtColor(current_img, current_img, cv::COLOR_BGR2GRAY);
	cv::cvtColor(src_img, src_img, cv::COLOR_BGR2GRAY);

	BlockMatrix src_dct_data;
	BlockMatrix wm_dct_data;
	DctTransformer::TransformImgToDct(src_img, src_dct_data);
	DctTransformer::TransformImgToDct(current_img, wm_dct_data);

	std::vector<double> extracted_wm;
	ExtractWatermark(src_dct_data, wm_dct_data, extracted_wm);

	cv::Mat binary_img(32, 32, CV_8UC1, cv::Scalar(255));

	for (int i = 0; i < 32; ++i) 
	{
		for (int j = 0; j < 32; ++j) 
		{
			int index = i * 32 + j;
			if (extracted_wm[index] < 0) 
				binary_img.at<uchar>(i, j) = 0;
			else 
				binary_img.at<uchar>(i, j) = 255;
		}
	}

	cv::cvtColor(binary_img, binary_img, cv::COLOR_GRAY2BGR);

	current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	src_img = RawDataConvert::RawDataToMat(raw_data_manager.GetSrcData());
	double ns_sim = NSProb(randomSequence, extracted_wm);
	PSNR psnr;
	double psnr_value = psnr.GetPSNR(src_img, current_img);

	binary_img.copyTo(current_img(cv::Rect(0, 0, binary_img.cols, binary_img.rows)));
	cv::imshow(cv::format("NSProb:%.2f%%, PSNR:%f", ns_sim, psnr_value), current_img);
}


void CImageProcessFrameworkDlg::JPEGTest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (raw_data_manager.GetCurrentData() == nullptr)
	{
		MessageBox(_T("No Image"));
		return;
	}

	cv::Mat current_img = RawDataConvert::RawDataToMat(raw_data_manager.GetCurrentData());
	cv::Mat src_img = RawDataConvert::RawDataToMat(raw_data_manager.GetSrcData());

	for (int i = 9; i > 0; i--)
	{
		std::vector<uchar> compressed_data;
		std::vector<int> compression_params = { cv::IMWRITE_JPEG_QUALITY, i*10 }; // JPEG 품질 지정
		cv::imencode(".jpg", current_img, compressed_data, compression_params);
		cv::Mat decodedImage = cv::imdecode(compressed_data, cv::IMREAD_COLOR);

		cv::Mat src;
		cv::Mat dec;
		cv::cvtColor(decodedImage, dec, cv::COLOR_BGR2GRAY);
		cv::cvtColor(src_img, src, cv::COLOR_BGR2GRAY);

		BlockMatrix src_dct_data;
		BlockMatrix wm_dct_data;
		DctTransformer::TransformImgToDct(src, src_dct_data);
		DctTransformer::TransformImgToDct(dec, wm_dct_data);

		std::vector<double> extracted_wm;
		ExtractWatermark(src_dct_data, wm_dct_data, extracted_wm);

		double ns_sim = NSProb(randomSequence, extracted_wm);
		PSNR psnr;
		double psnr_value = psnr.GetPSNR(src_img, decodedImage);
		cv::imshow(cv::format("JPEG: %d%%, NSProb:%.2f%%, PSNR:%f", i * 10 ,ns_sim, psnr_value), decodedImage);
	}
}
