#include "pch.h"
#include "HistogramProcessor.h"

using namespace std;
using namespace cv;

void HistogramProcessor::InputImg(cv::Mat input)
{
	src = input;
	dst.create(src.rows, src.cols, src.type());
	memset(histogram, 0, sizeof(histogram));
	memset(cdf, 0, sizeof(cdf));
}

void HistogramProcessor::ShowHistogram(int ch)
{
	uchar* pd = src.data;
	int bpp = src.elemSize();

	if (!(ch < bpp))
	{
		ch = bpp - 1;
	}

	GetHistogram(ch);

	Mat hg_grp = Mat(MAX_PIX_VAL_SIZE, MAX_PIX_VAL_SIZE, CV_8UC3);
	memset(hg_grp.data, 0, hg_grp.total() * hg_grp.elemSize());

	double max_value = 0;
	for (int i = 0; i < MAX_PIX_VAL_SIZE; i++)
		if (histogram[ch][i] > max_value)
			max_value = histogram[ch][i];

	Scalar bar_color;
	if (ch == 0)
		bar_color = Scalar(MAX_PIX_VAL_SIZE-1, 0, 0);
	else if (ch == 1)
		bar_color = Scalar(0, MAX_PIX_VAL_SIZE-1, 0);
	else if (ch == 2)
		bar_color = Scalar(0, 0, MAX_PIX_VAL_SIZE-1);
	for (int i = 0; i < MAX_PIX_VAL_SIZE; i++)
	{
		int bar_len = (double)MAX_PIX_VAL_SIZE * (histogram[ch][i] / max_value);
		rectangle(hg_grp, Point(i, MAX_PIX_VAL_SIZE-1), Point(i, MAX_PIX_VAL_SIZE-1 - bar_len), bar_color);
	}
	

	imshow(format("%s%d", "histogram", ch), hg_grp);
}

void HistogramProcessor::MatchPiesewiselinearHistogram(Point p1, Point p2, Point p3)
{
	auto calculate_pdf_func = [p1, p2, p3](int x)
		{
			double slope1;
			double slope2;

			if(p2.x - p1.x == 0)
				slope1 = 0;
			else
				slope1 = (p2.y - p1.y) / (p2.x - p1.x);

			if (p3.x - p2.x==0)
				slope2 = 0;
			else
				slope2 = (p3.y - p2.y) / (p3.x - p2.x);

			double y_itc1 = p1.y - slope1 * p1.x;
			double y_itc2 = p2.y - slope2 * p2.x;

			double y1 = slope1 * x + y_itc1;
			double y2 = slope2 * x + y_itc2;

			if (x >= p1.x && x <= p2.x) {
				return y1;
			}
			else if (x > p2.x && x <= p3.x) {
				return y2;
			}
		};

	auto calculate_pdf_cdf = [this, calculate_pdf_func]()
		{
			pdf_cdf[0] = calculate_pdf_func(0);
			for (int i = 1; i < MAX_PIX_VAL_SIZE; i++)
				pdf_cdf[i] = calculate_pdf_func(i) + pdf_cdf[i - 1];

			for (int i = 0; i < MAX_PIX_VAL_SIZE; i++)
				pdf_cdf[i] /= pdf_cdf[MAX_PIX_VAL_SIZE-1];
		};

	auto calculate_matching_table = [this](int ch)
		{
			for (int i = 0; i < MAX_PIX_VAL_SIZE; i++)
			{
				int l = 0;
				while (pdf_cdf[l++] < cdf[ch][i]);

				if(l== MAX_PIX_VAL_SIZE)
					matching_table[ch][i] = l - 1;
				else if (cdf[ch][i] - pdf_cdf[l - 1] < pdf_cdf[l] - cdf[ch][i])
					matching_table[ch][i] = l-1;
				else
					matching_table[ch][i] = l;
			}
		};

	calculate_pdf_cdf();
	for (int i = 0; i < src.elemSize(); i++)
		GetCDF(i);

	for (int i = 0; i < src.elemSize(); i++)
		calculate_matching_table(i);

	auto dd = dst.data;
	auto sd = src.data;

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			int pix_idx = (y * src.cols + x) * src.elemSize();
			for (int i = 0; i < src.elemSize(); i++)
				dd[pix_idx + i] = matching_table[i][sd[pix_idx + i]];
		}
	}
}

void HistogramProcessor::GetResult(Mat& output)
{
	output = dst.clone();
}

void HistogramProcessor::GetHistogram(int ch)
{
	uchar* pd = src.data;
	int bpp = src.elemSize();

	if (!(ch < bpp))
	{
		ch = bpp - 1;
	}

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			int pix_idx = (y * src.cols + x) * src.elemSize() + ch;
			histogram[ch][pd[pix_idx]]++;
		}
	}
}

void HistogramProcessor::GetCDF(int ch)
{
	GetHistogram(ch);
	cdf[ch][0] = histogram[ch][0];
	for (int i = 1; i < MAX_PIX_VAL_SIZE; i++)
		cdf[ch][i] = cdf[ch][i - 1] + histogram[ch][i];
	for (int i = 0; i < MAX_PIX_VAL_SIZE; i++)
		cdf[ch][i] /= cdf[ch][MAX_PIX_VAL_SIZE-1];
}