#pragma once

#include "CoordinatesStruct.h"

class Interpolation abstract
{
public:
	virtual int operator()(cv::Mat src, double src_crd_x, double src_crd_y,int channel) = 0;
	static int GetPixelIndex(cv::Mat src, int x, int y, int channel);
	static int GetPixelIndex(cv::Mat src, cv::Point pos, int channel);
};