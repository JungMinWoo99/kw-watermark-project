#include "pch.h"
#include "ImgDataManager.h"

using namespace std;

void ImgDataManager::InputNewSrc(cv::Mat src_img)
{
	while (!undo_stack.empty())
		undo_stack.pop();

	uchar* mat_data = src_img.data;
	uchar* cpy_data = new uchar[src_img.total() * src_img.elemSize()];

	memcpy(cpy_data, mat_data, src_img.total() * src_img.elemSize());

	src_data = make_shared<ImgRawData>();
	ImgRawData* raw_src_data = src_data.get();
	raw_src_data->byte_per_pixel = src_img.elemSize();
	raw_src_data->height = src_img.rows;
	raw_src_data->width = src_img.cols;
	raw_src_data->color_space = ImgRawData::Colorspace::BGR;//Mat의 기본형식이 BGR이므로
	raw_src_data->raw_data = cpy_data;

	undo_stack.push(src_data);
}

void ImgDataManager::PushRawData(std::shared_ptr<ImgRawData> prc_data)
{
	undo_stack.push(prc_data);
}

void ImgDataManager::UndoProcess()
{
	if (undo_stack.size()>1)
		undo_stack.pop();
}

shared_ptr<ImgRawData> ImgDataManager::GetCurrentData()
{
	if (!undo_stack.empty())
		return undo_stack.top();
	else
		return nullptr;
}

shared_ptr<ImgRawData> ImgDataManager::GetSrcData()
{
	return src_data;
}

cv::Mat RawDataConvert::RawDataToMat(std::shared_ptr<ImgRawData> src)
{
	auto current_raw_data = src;

	cv::Mat ret;
	int img_type;
	if (current_raw_data->byte_per_pixel == 1)
		img_type = CV_8UC1;
	else if (current_raw_data->byte_per_pixel == 3)
		img_type = CV_8UC3;
	else
	{
		cout << "this img type not support" << endl;
		img_type = CV_8UC1;
	}

	ret.create(current_raw_data->height, current_raw_data->width, img_type);
	int dataSize = current_raw_data->width * current_raw_data->height * current_raw_data->byte_per_pixel;
	memcpy(ret.data, current_raw_data->raw_data, dataSize);

	return ret;
}

std::shared_ptr<ImgRawData> RawDataConvert::MatToRawData(cv::Mat src, ImgRawData::Colorspace clr)
{
	std::shared_ptr<ImgRawData> ret = std::make_shared<ImgRawData>();

	// 이미지 데이터 복사
	ret->width = src.cols;
	ret->height = src.rows;
	ret->byte_per_pixel = src.channels();
	ret->color_space = clr;
	int dataSize = ret->width * ret->height * ret->byte_per_pixel;
	ret->raw_data = new unsigned char[dataSize];
	memcpy(ret->raw_data, src.data, dataSize);

	return ret;
}

shared_ptr<ImgRawData> RawDataConvert::BGRToGray(shared_ptr<ImgRawData> src)
{
	auto src_info = src.get();
	uchar* input_data = src_info->raw_data;
	int output_bpp = 1;
	uchar* output_data = new uchar[src_info->width * src_info->height * output_bpp];

	for (int y = 0; y < src_info->height; y++)
	{
		for (int x = 0; x < src_info->width; x++)
		{
			unsigned int input_pix_idx = (y * src_info->width + x) * src_info->byte_per_pixel;
			unsigned int output_pix_idx = (y * src_info->width + x) * output_bpp;

			//opencv는 BGR형식을 사용하므로 BGR순서로 픽셀값을 얻어야함
			double b_value = input_data[input_pix_idx];
			double g_value = input_data[input_pix_idx + 1];
			double r_value = input_data[input_pix_idx + 2];

			double y_value = 0.299 * r_value + 0.587 * g_value + 0.114 * b_value;

			output_data[output_pix_idx] = (unsigned char)y_value;
		}
	}

	shared_ptr<ImgRawData> ret = make_shared<ImgRawData>();
	auto ret_info = ret.get();
	ret_info->byte_per_pixel = output_bpp;
	ret_info->height = src_info->height;
	ret_info->width = src_info->width;
	ret_info->color_space = ImgRawData::Colorspace::Gray;
	ret_info->raw_data = output_data;

	return ret;
}

shared_ptr<ImgRawData> RawDataConvert::BGRToYUV444(shared_ptr<ImgRawData> src)
{
	auto src_info = src.get();
	uchar* input_data = src_info->raw_data;
	int output_bpp = 3;
	uchar* output_data = new uchar[src_info->width * src_info->height * output_bpp];

	for (int y = 0; y < src_info->height; y++)
	{
		for (int x = 0; x < src_info->width; x++)
		{
			unsigned int input_pix_idx = (y * src_info->width + x) * src_info->byte_per_pixel;
			unsigned int output_pix_idx = (y * src_info->width + x) * output_bpp;

			//opencv는 BGR형식을 사용하므로 BGR순서로 픽셀값을 얻어야함
			double b_value = input_data[input_pix_idx];
			double g_value = input_data[input_pix_idx + 1];
			double r_value = input_data[input_pix_idx + 2];

			double y_value = 0.299 * r_value + 0.587 * g_value + 0.114 * b_value;
			double u_value = 0.564 * (b_value - y_value) - 128;
			double v_value = 0.713 * (r_value - y_value) - 128;

			output_data[output_pix_idx] = (unsigned char)y_value;
			output_data[output_pix_idx + 1] = (unsigned char)u_value;
			output_data[output_pix_idx + 2] = (unsigned char)v_value;
		}
	}

	shared_ptr<ImgRawData> ret = make_shared<ImgRawData>();
	auto ret_info = ret.get();
	ret_info->byte_per_pixel = 3;
	ret_info->height = src_info->height;
	ret_info->width = src_info->width;
	ret_info->color_space = ImgRawData::Colorspace::YUV444;
	ret_info->raw_data = output_data;

	return ret;
}

shared_ptr<ImgRawData> RawDataConvert::BGRToRGBProgressive(shared_ptr<ImgRawData> src)
{
	auto src_info = src.get();
	uchar* input_data = src_info->raw_data;
	int output_bpp = 3;
	uchar* output_data = new uchar[src_info->width * src_info->height * output_bpp];

	for (int y = 0; y < src_info->height; y++)
	{
		for (int x = 0; x < src_info->width; x++)
		{
			unsigned int input_pix_idx = (y * src_info->width + x) * src_info->byte_per_pixel;
			unsigned int output_pix_idx = (y * src_info->width) * output_bpp + x;

			//opencv는 BGR형식을 사용하므로 BGR순서로 픽셀값을 얻어야함
			double b_value = input_data[input_pix_idx];
			double g_value = input_data[input_pix_idx + 1];
			double r_value = input_data[input_pix_idx + 2];

			output_data[output_pix_idx] = r_value;
			output_data[output_pix_idx + src_info->width] = g_value;
			output_data[output_pix_idx + 2 * src_info->width] = b_value;
		}
	}

	shared_ptr<ImgRawData> ret = make_shared<ImgRawData>();
	auto ret_info = ret.get();
	ret_info->byte_per_pixel = 1;
	ret_info->height = src_info->height;
	ret_info->width = src_info->width* output_bpp;
	ret_info->color_space = ImgRawData::Colorspace::RGB_progressive;
	ret_info->raw_data = output_data;

	return ret;
}