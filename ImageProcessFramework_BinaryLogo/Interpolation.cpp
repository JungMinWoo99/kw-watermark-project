#include "pch.h"
#include "Interpolation.h"

using namespace cv;
using namespace std;

int Interpolation::GetPixelIndex(Mat src, int x, int y, int channel)
{
	return (y * src.cols + x) * src.elemSize()+ channel;
}

int Interpolation::GetPixelIndex(Mat src, Point pos, int channel)
{
	return (pos.y * src.cols + pos.x) * src.elemSize()+ channel;
}