#pragma once

#include "Interpolation.h"
#include <cmath>

class BicubicInterpolation :Interpolation
{
public:
	int operator()(cv::Mat src, double src_crd_x, double src_crd_y, int channel);
private:
	AdjacentCoordinates16 GetAdjacentCoordinates(double x, double y);

	double CubicInterpolation(double* p_value, double start_pos, double target_pos);
};