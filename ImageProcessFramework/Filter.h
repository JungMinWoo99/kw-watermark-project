#pragma once

#include <opencv2/opencv.hpp>

class Filter
{
private:
	cv::Mat src;
	cv::Mat padding_src;
	cv::Mat dst;

	double** filter_kernel;
	int kernel_size;

	double** kernel_impulse_response;

public:
	void InputImg(cv::Mat& src);

	void InputFilter(double** filter_kernel, int kernel_size);

	void FilterImg();

	void MedianFilter(int size);

	void GeometricMeanFilter(int size);

	void AdaptiveMedianFilter(int max_size);

	void GetFilterReslut(cv::Mat& output);

	void Threshold(cv::Mat& src, cv::Mat& dst, int t1, int t2);
};

