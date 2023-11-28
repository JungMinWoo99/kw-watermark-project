#include "pch.h"
#include "BilinearInterpolation.h"

using namespace cv;
using namespace std;

int BilinearInterpolation::operator()(cv::Mat src, double src_crd_x, double src_crd_y, int channel)
{
	AdjacentCoordinates4 adj_crd = GetAdjacentCoordinates(src_crd_x, src_crd_y);
	double x_ratio = src_crd_x - adj_crd.p[0].x;//adj_crd.p[1].x - adj_crd.p[0].x은 1 또는 0 이므로 나누기 생략
	double y_ratio = src_crd_y - adj_crd.p[0].y;

	unsigned int p_value[4];
	for (int i = 0; i < 4; i++)
	{
		if (-1 < adj_crd.p[i].x && adj_crd.p[i].x < src.cols && -1 < adj_crd.p[i].y && adj_crd.p[i].y < src.rows)
		{
			int pix_data_idx = GetPixelIndex(src, adj_crd.p[i], channel);
			p_value[i] = src.data[pix_data_idx];
		}
		else
			p_value[i] = 0;
	}

	//2차원 보간 값 계산
	double linear_interpolation_value_1 = (1 - x_ratio) * p_value[0] + x_ratio * p_value[1];
	double linear_interpolation_value_2 = (1 - x_ratio) * p_value[2] + x_ratio * p_value[3];
	double linear_interpolation_value_3 = (1 - y_ratio) * linear_interpolation_value_1 + y_ratio * linear_interpolation_value_2;

	return round(linear_interpolation_value_3);
}

AdjacentCoordinates4 BilinearInterpolation::GetAdjacentCoordinates(double x, double y)
{
	AdjacentCoordinates4 ret;
	ret.p[0] = Point(floor(x), floor(y));
	ret.p[1] = Point(ceil(x), floor(y));
	ret.p[2] = Point(floor(x), ceil(y));
	ret.p[3] = Point(ceil(x), ceil(y));
	
	return ret;
}

