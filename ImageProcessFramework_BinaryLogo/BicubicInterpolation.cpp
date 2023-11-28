#include "pch.h"
#include "BicubicInterpolation.h"

using namespace cv;
using namespace std;
int BicubicInterpolation::operator()(cv::Mat src, double src_crd_x, double src_crd_y, int channel)
{
	AdjacentCoordinates16 adj_crd = GetAdjacentCoordinates(src_crd_x, src_crd_y);

	double p_value[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int l = 0; l < 4; l++)
		{
			if (-1 < adj_crd.p[i].x && adj_crd.p[i].x < src.cols && -1 < adj_crd.p[i].y && adj_crd.p[i].y < src.rows)
			{
				int pix_data_idx = GetPixelIndex(src, adj_crd.p[i * 4 + l], channel);
				p_value[i][l] = src.data[pix_data_idx];
			}
			else
				p_value[i][l] = 0;
		}
	}

	double cubic_interpolation_value[4];
	for (int i = 0; i < 4; i++)
		cubic_interpolation_value[i] = CubicInterpolation(p_value[i], adj_crd.p[i * 4].x, src_crd_x);
	
	int ret = round(CubicInterpolation(cubic_interpolation_value, adj_crd.p[0].y, src_crd_y));
	
	if (ret < 0)
		return 0;
	if (ret > 255)
		return 255;

	return ret;
}

double BicubicInterpolation::CubicInterpolation(double* p_value, double start_pos, double target_pos)
{
	//��׶��� ���������� ���ϱ�
	/*
	* 4���� ��ǥ�� �����̵� ���� 0,1,2,3���� �����ϸ� �����Լ��� ����� ���ϱ� ���� �ʿ��� ������� ����� ������ �� ����
	*/
	double a = (-1.0 / 6.0) * p_value[0] + (1.0 / 2.0) * p_value[1] + (-1.0 / 2.0) * p_value[2] + (1.0 / 6.0) * p_value[3];
	double b = (1.0) * p_value[0] + (-5.0 / 2.0) * p_value[1] + (2.0) * p_value[2] + (-1.0 / 2.0) * p_value[3];
	double c = (-11.0 / 6.0) * p_value[0] + (3.0) * p_value[1] + (-3.0 / 2.0) * p_value[2] + (1.0 / 3.0) * p_value[3];
	double d = (1.0) * p_value[0];
	double ret = a * pow(target_pos - start_pos, 3) + b * pow(target_pos - start_pos, 2) + c * (target_pos - start_pos) + d;
	return ret;
}

AdjacentCoordinates16 BicubicInterpolation::GetAdjacentCoordinates(double x, double y)
{
	AdjacentCoordinates16 ret;
	ret.p[0] = Point(floor(x) - 1, floor(y) - 1);
	ret.p[1] = Point(floor(x), floor(y) - 1);
	ret.p[2] = Point(ceil(x), floor(y) - 1);
	ret.p[3] = Point(ceil(x) + 1, floor(y) - 1);
	ret.p[4] = Point(floor(x) - 1, floor(y));
	ret.p[5] = Point(floor(x), floor(y));
	ret.p[6] = Point(ceil(x), floor(y));
	ret.p[7] = Point(ceil(x) + 1, floor(y));
	ret.p[8] = Point(floor(x) - 1, ceil(y));
	ret.p[9] = Point(floor(x), ceil(y));
	ret.p[10] = Point(ceil(x), ceil(y));
	ret.p[11] = Point(ceil(x) + 1, ceil(y));
	ret.p[12] = Point(floor(x) - 1, ceil(y) + 1);
	ret.p[13] = Point(floor(x), ceil(y) + 1);
	ret.p[14] = Point(ceil(x), ceil(y) + 1);
	ret.p[15] = Point(ceil(x) + 1, ceil(y) + 1);

	return ret;
}