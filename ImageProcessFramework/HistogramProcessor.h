#pragma once

#include <opencv2/opencv.hpp>

#define MAX_PIX_VAL_SIZE 256
#define MAX_CHANNEL 3

class HistogramProcessor
{
public:
	void InputImg(cv::Mat input);

	void ShowHistogram(int ch);

	void MatchPiesewiselinearHistogram(cv::Point p1 = cv::Point(MAX_PIX_VAL_SIZE / 2, MAX_PIX_VAL_SIZE / 2), cv::Point p2 = cv::Point(MAX_PIX_VAL_SIZE /2, MAX_PIX_VAL_SIZE /2), cv::Point p3= cv::Point(MAX_PIX_VAL_SIZE / 2, MAX_PIX_VAL_SIZE / 2));

	void GetResult(cv::Mat& output);
private:
	cv::Mat src;
	cv::Mat dst;
	int histogram[MAX_CHANNEL][MAX_PIX_VAL_SIZE];
	double cdf[MAX_CHANNEL][MAX_PIX_VAL_SIZE];
	double pdf_cdf[MAX_PIX_VAL_SIZE];
	int matching_table[MAX_CHANNEL][MAX_PIX_VAL_SIZE];

	void GetHistogram(int ch);
	void GetCDF(int ch);

};

