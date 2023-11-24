#pragma once
#include "DCTTransformer.h"
#include <vector>

//���������� ����
std::vector<double> generateRandomSequence();

//���͸�ũ ����
void WatermarkInsert(BlockMatrix& dct_data, std::vector<double>& randomSequence);

//���͸�ũ ����
void ExtractWatermark(BlockMatrix& org, BlockMatrix& dct_data, std::vector<double>& sequence);

//���絵 ���
double NSProb(const std::vector<double>& org, const std::vector<double>& sequence);