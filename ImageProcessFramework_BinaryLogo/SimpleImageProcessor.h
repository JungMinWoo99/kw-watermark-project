#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <opencv2/opencv.hpp>
#include <functional>

#include "CoordinatesStruct.h"

class ImageMaster
{
public:
	cv::Mat ResizeImage(cv::Mat src, unsigned int resize_x, unsigned resize_y, std::function<int(cv::Mat, double, double, int)> interpolation_func);

	cv::Mat RotateImage(cv::Mat src, int rotation_degrees, std::function<int(cv::Mat, double, double, int)> interpolation_func);

	cv::Mat LinearContrastStretching(cv::Mat src, cv::Point p1, cv::Point p2);
private:
	std::pair<double, double> RotateCoordinate(cv::Mat src, int x, int y, int degrees);
};