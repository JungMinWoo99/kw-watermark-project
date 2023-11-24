#pragma once
#include "BlockMatrix.h"
#include <opencv2/opencv.hpp>

class DctTransformer
{
private:

    static void ResizeAndPad(cv::Mat& input, cv::Mat& dst);

    static void SplitImg(cv::Mat& padded, BlockMatrix& block_data);

    static void DctBlockVector(BlockMatrix& src, BlockMatrix& dst);

    static void Quantization(BlockMatrix& src, BlockMatrix& dst);

    static void InverseQuantization(BlockMatrix& src, BlockMatrix& dst);

    static void IdctBlockVector(BlockMatrix& src, BlockMatrix& dst);

    static void MergeBlock(BlockMatrix& block_data, cv::Mat& merged);

public:
    //밝기를 양자화 하는 테이블
    static const double luminanceValues[BLOCK_SIZE][BLOCK_SIZE];

    static void TransformImgToDct(cv::Mat& src, BlockMatrix& dst);

    static void TransformDctToImg(BlockMatrix& src, cv::Mat& dst);
};