
// ImageProcessFrameworkDlg.h: 헤더 파일
//

#pragma once
#include "ImgDataManager.h"

// CImageProcessFrameworkDlg 대화 상자
class CImageProcessFrameworkDlg : public CDialogEx
{
// 생성입니다.
public:
	CImageProcessFrameworkDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEPROCESSFRAMEWORK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
private:
	ImgDataManager raw_data_manager;
	std::vector<double> randomSequence;

	CImage screen;

	void PrintScreen();
	void PrintView(std::shared_ptr<ImgRawData> print_img);

	void SaveRawData(std::string save_file, uchar* save_data, size_t data_size);
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OpenImageFile();

	afx_msg void Undo();

	afx_msg void ViewScaled();
	afx_msg void ViewGray();
	afx_msg void ViewRGBProgressive();
	afx_msg void ViewYUV444();
	
	afx_msg void SaveAsGray();
	afx_msg void SaveASYUV444();
	afx_msg void SaveAsRGBProgressive();

	afx_msg void ResizeProcess();
	afx_msg void RotateProcess();
	afx_msg void MatchHitogram();
	afx_msg void ShowHistogram();
	afx_msg void AvgFilter();
	afx_msg void GaussianFilter();
	afx_msg void LinearContrastStretching();
	afx_msg void AddGaussianNoise();
	afx_msg void AddSnPNoise();
	afx_msg void GeometricMeanFilter();
	afx_msg void MedianFilter();
	afx_msg void AdaptiveMedianFilter();
	afx_msg void Thresholding();
	afx_msg void ViewPSNR();
	afx_msg void InsertWM();
	afx_msg void ExtractWM();
	afx_msg void JPEGTest();
};
