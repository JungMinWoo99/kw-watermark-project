#include "pch.h"
#include "PSNR.h"

double PSNR::GetPSNR(cv::Mat src, cv::Mat dst)
{

	auto get_mse = [&]() {
		double mse = 0;
		for (int n = 0; n < src.rows; n++)
		{
			for (int m = 0; m < src.cols; m++)
			{
				int pix_idx = (n * src.cols + m) * src.elemSize();
				for (int i = 0; i < src.elemSize(); i++)
				{
					double src_value = src.data[pix_idx + i];
					double dst_value = dst.data[pix_idx + i];
					mse += pow(src_value - dst_value, 2);
				}
			}
		}
		mse /= src.rows * src.cols * src.elemSize();

		return mse;
		};

	auto ret = 10.0 * log10(pow(255, 2) / get_mse());
	return ret;
}
