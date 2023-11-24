#pragma once
#include "DCTTransformer.h"
#include <vector>

//랜덤시퀀스 생성
std::vector<double> generateRandomSequence();

//워터마크 삽입
void WatermarkInsert(BlockMatrix& dct_data, std::vector<double>& randomSequence);

//워터마크 추출
void ExtractWatermark(BlockMatrix& org, BlockMatrix& dct_data, std::vector<double>& sequence);

//유사도 계산
double NSProb(const std::vector<double>& org, const std::vector<double>& sequence);