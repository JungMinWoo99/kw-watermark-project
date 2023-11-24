#pragma once
#include <opencv2/opencv.hpp>

class PSNR 
{
public:
	static double GetPSNR(cv::Mat src, cv::Mat dst);
};