#pragma once

#include "Interpolation.h"

class BilinearInterpolation : Interpolation
{
public:
	int operator()(cv::Mat src, double src_crd_x, double src_crd_y, int channel);
private:
	AdjacentCoordinates4 GetAdjacentCoordinates(double x, double y);
};