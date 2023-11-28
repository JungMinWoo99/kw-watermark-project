#include "pch.h"
#include "Noise.h"

#define _USE_MATH_DEFINES 
#include <math.h>
#include <random>
#include <windows.h>

using namespace std;
using namespace cv;

GaussianNoise::GaussianNoise(float mean, float std_dev)
{
	srand(GetTickCount());
	GetPDF(mean, std_dev);
	GetCDF();
}

void GaussianNoise::AddNoise(cv::Mat src)
{	
	for(int i=0;i<src.elemSize();i++)
	{
		for (int n = 0; n < src.rows; n++)
		{
			for (int m = 0; m < src.cols; m++)
			{
				int pix_idx = (n * src.cols + m) * src.elemSize() + i;
				int tem = (int)src.data[pix_idx] + GetNoise();
				if (tem < 0)
					src.data[pix_idx] = 0;
				else if (tem > 255)
					src.data[pix_idx] = 255;
				else
					src.data[pix_idx] = tem;
			}
		}
	}
}

void GaussianNoise::GetPDF(float mean, float std_dev)
{
	int center = PDF_LEN / 2;
	for (int i = 0; i < PDF_LEN; i++)
	{
		float x = i - center;
		pdf[i] = (1 / ((float)sqrt(2 * M_PI) * std_dev)) * exp(-pow(x - mean, 2) / (2 * std_dev * std_dev));
	}
}

void GaussianNoise::GetCDF()
{
	cdf[0] = pdf[0]/2;
	for (int i = 1; i < PDF_LEN; i++)
		cdf[i] = (pdf[i]+pdf[i-1])/2 + cdf[i - 1];
}

int GaussianNoise::GetNoise()
{
	float f_rand = (float)rand() / (RAND_MAX + 1);
	int i = 0;
	while (i != PDF_LEN && f_rand > cdf[i++]);
	return i - 1 - PDF_LEN / 2;
}

SnPNoise::SnPNoise(float s_prob, float p_prob): s_prob(s_prob), p_prob(p_prob)
{
	srand(GetTickCount());
}

void SnPNoise::AddNoise(cv::Mat src)
{
	for (int i = 0; i < src.elemSize(); i++)
	{
		for (int n = 0; n < src.rows; n++)
		{
			for (int m = 0; m < src.cols; m++)
			{
				int pix_idx = (n * src.cols + m) * src.elemSize() + i;
				int noise = GetNoise();
				if (noise != true_value)
					src.data[pix_idx] = noise;
			}
		}
	}
}

int SnPNoise::GetNoise()
{
	float low = s_prob;
	float high = 1.0f - p_prob;

	float f_rand = ((float)rand() / RAND_MAX);
	
	if (f_rand < low)
		return 255;
	else if (f_rand > high)
		return 0;
	else
		return true_value;
}

