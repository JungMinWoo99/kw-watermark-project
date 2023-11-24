#include "pch.h"
#include "SimpleImageProcessor.h"

using namespace cv;
using namespace std;

Mat ImageMaster::ResizeImage(Mat src, unsigned int resize_x, unsigned resize_y, function<int(Mat, double, double,int )> interpolation_func)
{
	Mat output;
	output.create(resize_y, resize_x, src.type());
	int byte_per_pixel = src.elemSize();
	uchar* output_data = output.data;

	//계산에 필요한 값
	double x_scale = (double)resize_x / src.cols;
	double y_scale = (double)resize_y / src.rows;

	for (int i = 0; i < byte_per_pixel; i++)
	{
		for (int y = 0; y < output.rows; y++)
		{
			for (int x = 0; x < output.cols; x++)
			{
				double src_crd_x = x / x_scale;
				double src_crd_y = y / y_scale;

				int pix_idx = (y * output.cols + x) * output.channels() + i;
				
				output_data[pix_idx] = interpolation_func(src, src_crd_x, src_crd_y, i);
			}
		}
	}

	return output;
}

Mat ImageMaster::RotateImage(Mat src, int rotation_degrees, function<int(Mat, double, double, int)> interpolation_func)
{
	Mat output;
	output.create(src.rows, src.cols, src.type());
	int byte_per_pixel = src.elemSize();
	uchar* output_data = output.data;

	for (int i = 0; i < byte_per_pixel; i++)
	{
		for (int y = 0; y < output.rows; y++)
		{
			for (int x = 0; x < output.cols; x++)
			{
				auto src_crd = RotateCoordinate(output, x, y, -rotation_degrees);//회전하기 전의 좌표
				int pix_idx = (y * output.cols + x) * output.channels() + i;

				if (src_crd.first >= 0 && src_crd.first <= src.cols && src_crd.second >= 0 && src_crd.second <= src.rows)
				{
					double src_crd_x = src_crd.first;
					double src_crd_y = src_crd.second;

					output_data[pix_idx] = interpolation_func(src, src_crd_x, src_crd_y, i);
				}
				else
					output_data[pix_idx] = 0;
			}
		}
	}

	return output;
}

Mat ImageMaster::LinearContrastStretching(Mat src, Point p1, Point p2)
{
	//특정 컬러만 스트래칭할 수 있도록 약간 변경하기
	Mat output;
	output.create(src.rows, src.cols, src.type());
	int byte_per_pixel = src.elemSize();

	double line1_slope = (double)p1.y / p1.x;
	double line2_slope = (double)(p2.y - p1.y) / (p2.x - p1.x);
	double line3_slope = (double)(255 - p2.y) / (255 - p2.x);

	for (int i = 0; i < byte_per_pixel; i++)
	{
		for (int y = 0; y < src.rows; y++)
		{
			for (int x = 0; x < src.cols; x++)
			{
				int pix_idx = (y * src.cols + x) * output.elemSize() + i;

				double pix_value = src.data[pix_idx];
				if (pix_value < p1.x)
				{
					output.data[pix_idx] = pix_value * line1_slope;
				}
				else if (pix_value > p2.x)
				{
					output.data[pix_idx] = (pix_value - p2.x) * line3_slope + p2.y;
				}
				else
				{
					output.data[pix_idx] = (pix_value - p1.x) * line2_slope + p1.y;
				}
			}
		}
	}

	return output;
}

pair<double, double> ImageMaster::RotateCoordinate(Mat src, int x, int y, int degrees)
{
	double mid_x = (double)src.cols / 2;
	double mid_y = (double)src.rows / 2;
	double radian = degrees * M_PI / 180;

	double rotated_x = cos(radian) * (x - mid_x) - sin(radian) * (y - mid_y) + mid_x;
	double rotated_y = sin(radian) * (x - mid_x) + cos(radian) * (y - mid_y) + mid_y;

	return make_pair(rotated_x, rotated_y);
}
