#pragma once

#include <opencv2/opencv.hpp>
#include <memory>
#include <functional>
#include <stack>

struct ImgRawData
{
	enum Colorspace
	{
		BGR,
		RGB_progressive,
		YUV444,
		Gray
	};

	short byte_per_pixel;
	int height;
	int width;
	Colorspace color_space;

	uchar* raw_data;

	~ImgRawData()
	{
		delete[] raw_data;
	}
};

class ImgDataManager
{
private:
	std::shared_ptr<ImgRawData> src_data;
	std::stack<std::shared_ptr<ImgRawData>> undo_stack;
public:
	void InputNewSrc(cv::Mat src_img);

	void PushRawData(std::shared_ptr<ImgRawData> prc_data);

	void UndoProcess();

	std::shared_ptr<ImgRawData> GetCurrentData();

	std::shared_ptr<ImgRawData> GetSrcData();
};

namespace RawDataConvert
{
	cv::Mat RawDataToMat(std::shared_ptr<ImgRawData> src);
	std::shared_ptr<ImgRawData> MatToRawData(cv::Mat src, ImgRawData::Colorspace);
	std::shared_ptr<ImgRawData> BGRToGray(std::shared_ptr<ImgRawData> src);
	std::shared_ptr<ImgRawData> BGRToYUV444(std::shared_ptr<ImgRawData> src);
	std::shared_ptr<ImgRawData> BGRToRGBProgressive(std::shared_ptr<ImgRawData> src);
};
