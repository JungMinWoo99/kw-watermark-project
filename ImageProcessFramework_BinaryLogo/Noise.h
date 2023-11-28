#pragma once

#include <opencv2/opencv.hpp>

#define PDF_LEN 256

class GaussianNoise
{
public:
	GaussianNoise(float mean, float std_dev);
	void AddNoise(cv::Mat src);

private:
	float pdf[PDF_LEN];
	float cdf[PDF_LEN];

	void GetPDF(float mean, float std_dev);
	void GetCDF();
	int GetNoise();
};

class SnPNoise
{
public:
	SnPNoise(float s_prob, float p_prob);
	void AddNoise(cv::Mat src);

private:
	static const uchar true_value = 128;

	float s_prob;
	float p_prob;

	int GetNoise();
};