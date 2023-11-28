#include "pch.h"

#include "Filter.h"
#include <vector>

using namespace std;
using namespace cv;

void Filter::InputImg(cv::Mat& src)
{
	this->src = src.clone();
}

void Filter::InputFilter(double** filter_kernel, int kernel_size)
{
	this->filter_kernel = filter_kernel;
	this->kernel_size = kernel_size;
	kernel_impulse_response = new double* [kernel_size];
	for (int i = 0; i < kernel_size; i++)
		kernel_impulse_response[i] = new double[kernel_size];
	for (int y = 0; y < kernel_size; y++)
	{
		for (int x = 0; x < kernel_size; x++)
			kernel_impulse_response[y][x] = filter_kernel[kernel_size - y - 1][kernel_size - x - 1];
	}
}

void Filter::FilterImg()
{
	
	auto FilterOneChannel = [this](int filter_channel) {
		int fc = filter_channel;
		double** ir = kernel_impulse_response;

		double* buf = new double [dst.total() * dst.elemSize()] {0, };

		for (int y = 0; y < padding_src.rows; y++)
		{
			for (int x = 0; x < padding_src.cols; x++)
			{
				int padding_src_idx = (y * padding_src.cols + x) * padding_src.elemSize() + fc;
				for (int n = 0; n < kernel_size; n++)
				{
					for (int m = 0; m < kernel_size; m++)
					{
						//int process_dst_x = x + (m - kernel_size / 2) - kernel_size / 2;
						int process_dst_x = x + m - kernel_size + 1;
						//int process_dst_y = y + (n - kernel_size / 2) - kernel_size / 2;
						int process_dst_y = y + n - kernel_size + 1;
						if (-1 < process_dst_x && process_dst_x < dst.cols && -1 < process_dst_y && process_dst_y < dst.rows)
						{
							int process_dst_pix_idx = (process_dst_y * dst.cols + process_dst_x) * dst.elemSize() + fc;
							buf[process_dst_pix_idx] += (double)padding_src.data[padding_src_idx] * ir[n][m];
						}
					}
				}
			}
		}

		for (int y = 0; y < dst.rows; y++)
		{
			for (int x = 0; x < dst.cols; x++)
			{
				int pix_idx = (y * dst.cols + x) * dst.elemSize() + fc;
				if (buf[pix_idx] < 0)
					dst.data[pix_idx] = 0;
				else if (buf[pix_idx] > 255)
					dst.data[pix_idx] = 255;
				else
					dst.data[pix_idx] = buf[pix_idx];
			}
		}
		delete[] buf;
	};
	

	dst.create(src.rows, src.cols, src.type());
	padding_src.create(src.rows + kernel_size - 1, src.cols + kernel_size - 1, src.type());
	uchar* padding_data = padding_src.data;
	uchar* src_data = src.data;

	//zero padding
	memset(padding_data, 0, padding_src.total() * padding_src.elemSize());
	memset(dst.data, 0, dst.total() * dst.elemSize());
	for (int y = 0; y < src.rows; y++)
	{
		int padding_idx = ((y + kernel_size / 2) * padding_src.cols + kernel_size / 2)* padding_src.elemSize();
		int src_idx = y * src.cols * src.elemSize();
		memcpy(padding_data + padding_idx, src_data + src_idx, src.cols * src.elemSize());
	}

	for (int i = 0; i < padding_src.elemSize(); i++)
	{
		FilterOneChannel(i);
	}
}

void Filter::MedianFilter(int size)
{
	auto FilterOneChannel = [this, size](int filter_channel) {
		int fc = filter_channel;

		vector<unsigned char> sort_vector;
		sort_vector.resize(size * size);

		for (int y = 0; y < dst.rows; y++)
		{
			for (int x = 0; x < dst.cols; x++)
			{
				int dst_idx = (y * dst.cols + x) * dst.elemSize() + fc;

				int i = 0;
				for (int n = y; n < y + size; n++)
				{
					for (int m = x; m < x + size; m++)
					{
						int padding_idx = (n * padding_src.cols + m) * padding_src.elemSize() + fc;
						sort_vector[i++] = padding_src.data[padding_idx];
					}
				}

				sort(sort_vector.begin(), sort_vector.end());
				dst.data[dst_idx] = sort_vector[size * size / 2];
			}
		}
		};

	dst.create(src.rows, src.cols, src.type());
	padding_src.create(src.rows + size - 1, src.cols + size - 1, src.type());
	uchar* padding_data = padding_src.data;
	uchar* src_data = src.data;

	//zero padding
	memset(padding_data, 0, padding_src.total() * padding_src.elemSize());
	memset(dst.data, 0, dst.total() * dst.elemSize());
	for (int y = 0; y < src.rows; y++)
	{
		int padding_idx = ((y + size / 2) * padding_src.cols + size / 2) * padding_src.elemSize();
		int src_idx = y * src.cols * src.elemSize();
		memcpy(padding_data + padding_idx, src_data + src_idx, src.cols * src.elemSize());
	}

	for (int i = 0; i < padding_src.elemSize(); i++)
	{
		FilterOneChannel(i);
	}
}

void Filter::GeometricMeanFilter(int size)
{
	auto FilterOneChannel = [this, size](int filter_channel) {
		int fc = filter_channel;

		for (int y = 0; y < dst.rows; y++)
		{
			for (int x = 0; x < dst.cols; x++)
			{
				int dst_idx = (y * dst.cols + x) * dst.elemSize() + fc;

				double pi = 0;
				for (int n = y; n < y + size; n++)
				{
					for (int m = x; m < x + size; m++)
					{
						int padding_idx = (n * padding_src.cols + m) * padding_src.elemSize() + fc;
						if (padding_src.data[padding_idx] != 0)
							pi += log( padding_src.data[padding_idx]);
					}
				}

				dst.data[dst_idx] = (uchar)exp(pi/(size*size));
			}
		}
		};

	dst.create(src.rows, src.cols, src.type());
	padding_src.create(src.rows + size - 1, src.cols + size - 1, src.type());
	uchar* padding_data = padding_src.data;
	uchar* src_data = src.data;

	//zero padding
	memset(padding_data, 0, padding_src.total() * padding_src.elemSize());
	memset(dst.data, 0, dst.total() * dst.elemSize());
	for (int y = 0; y < src.rows; y++)
	{
		int padding_idx = ((y + size / 2) * padding_src.cols + size / 2) * padding_src.elemSize();
		int src_idx = y * src.cols * src.elemSize();
		memcpy(padding_data + padding_idx, src_data + src_idx, src.cols * src.elemSize());
	}

	for (int i = 0; i < padding_src.elemSize(); i++)
	{
		FilterOneChannel(i);
	}
}

void Filter::AdaptiveMedianFilter(int max_size)
{
	auto FilterOneChannel = [this, max_size](int filter_channel) {
		int fc = filter_channel;

		vector<unsigned char> sort_vector;

		
		for (int y = 0; y < dst.rows; y++)
		{
			for (int x = 0; x < dst.cols; x++)
			{
				for (int filter_size = 3; filter_size <= max_size; filter_size += 2)
				{
					sort_vector.resize(filter_size * filter_size);

					int dst_idx = (y * dst.cols + x) * dst.elemSize() + fc;
					auto src_value = src.data[dst_idx];
					dst.data[dst_idx] = src_value;

					int i = 0;
					for (int n = y + (max_size - filter_size) / 2; n < y + filter_size; n++)
					{
						for (int m = x + (max_size - filter_size) / 2; m < x + filter_size; m++)
						{
							int padding_idx = (n * padding_src.cols + m) * padding_src.elemSize() + fc;
							sort_vector[i++] = padding_src.data[padding_idx];
						}
					}
					sort(sort_vector.begin(), sort_vector.end());

					auto min = sort_vector.front();
					auto mid = sort_vector[filter_size * filter_size / 2];
					auto max = sort_vector.back();
					if (min < mid && mid < max)
					{
						if (!(min < src_value && src_value < max))
							dst.data[dst_idx] = mid;
					}
				}
			}
		}
		};

	dst.create(src.rows, src.cols, src.type());
	padding_src.create(src.rows + max_size - 1, src.cols + max_size - 1, src.type());
	uchar* padding_data = padding_src.data;
	uchar* src_data = src.data;

	//zero padding
	memset(padding_data, 0, padding_src.total() * padding_src.elemSize());
	memset(dst.data, 0, dst.total() * dst.elemSize());
	for (int y = 0; y < src.rows; y++)
	{
		int padding_idx = ((y + max_size / 2) * padding_src.cols + max_size / 2) * padding_src.elemSize();
		int src_idx = y * src.cols * src.elemSize();
		memcpy(padding_data + padding_idx, src_data + src_idx, src.cols * src.elemSize());
	}

	for (int i = 0; i < padding_src.elemSize(); i++)
	{
		FilterOneChannel(i);
	}
}

void Filter::GetFilterReslut(Mat& output)
{
	dst.copyTo(output);
}

void Filter::Threshold(Mat& src, Mat& dst, int t1, int t2)
{
	if (t1 > t2)
	{
		int tem = t1;
		t1 = t2;
		t2 = t1;
	}
	cvtColor(src, src, COLOR_BGR2GRAY);
	dst.create(src.size(), src.type());
	for (int y = 0; y < dst.rows; y++)
	{
		for (int x = 0; x < dst.cols; x++)
		{
			int pix_idx = (y * dst.cols + x) * dst.elemSize();
			if (src.data[pix_idx] < t1)
				dst.data[pix_idx] = 0;
			else if (src.data[pix_idx] > t2)
				dst.data[pix_idx] = 255;
			else
				dst.data[pix_idx] = 128;
		}
	}

}