#include "DCTTransformer.h"
#include <math.h>
#define M_PIl 3.141592653589793238462643383279502884L /* pi */

using namespace cv;
using namespace std;

const double DctTransformer::luminanceValues[BLOCK_SIZE][BLOCK_SIZE] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};

void DctTransformer::ResizeAndPad(cv::Mat& input, cv::Mat& padded)
{
    auto get_optimal_dct_size = [](unsigned int size)
        {
            while ((size % 8) != 0)
                size++;
            return size;
        };
    int optimal_rows = get_optimal_dct_size(input.rows);
    int optimal_cols = get_optimal_dct_size(input.cols);

    padded.create(optimal_rows, optimal_cols, input.type());
    uchar* padded_data = padded.data;
    uchar* input_data = input.data;

    //zero padding
    memset(padded_data, 0, padded.total() * padded.elemSize());
    for (int y = 0; y < input.rows; y++)
    {
        int padded_idx = (y * padded.cols) * padded.elemSize();
        int input_idx = y * input.cols * input.elemSize();
        memcpy(padded_data + padded_idx, input_data + input_idx, input.cols * input.elemSize());
    }
}

void DctTransformer::SplitImg(Mat& padded, BlockMatrix& block_data)
{
    auto copy_block_data = [&](int y, int x) 
        {
            for (int n = 0; n < BLOCK_SIZE; n++)
            {
                for (int m = 0; m < BLOCK_SIZE; m++)
				{
					int pix_idx = (n + y * BLOCK_SIZE) * padded.cols + m + x * BLOCK_SIZE;
                    block_data(y, x)(n, m) = padded.data[pix_idx];
                }
            }
        };

    int rows_block_num = padded.rows / BLOCK_SIZE;
    int cols_block_num = padded.cols / BLOCK_SIZE;

    block_data.Resize(rows_block_num, cols_block_num);

    for (int n = 0; n < rows_block_num; n++)
    {
        for (int m = 0; m < cols_block_num; m++)
            copy_block_data(n, m);
    }
}

void DctTransformer::DctBlockVector(BlockMatrix& src, BlockMatrix& dst)
{
    auto dct = [&](int y, int x)
        {
            //오픈 소스의 dct코드를 그대로 가져옴
            auto getCval = [](int y, int x) 
                {
                if (y == 0 && x == 0)
                    return 1.0 / static_cast<double>(BLOCK_SIZE);
                else if (y != 0 && x != 0)
                    return 2.0 / static_cast<double>(BLOCK_SIZE);
                else
                    return sqrt(2.0) / static_cast<double>(BLOCK_SIZE);
                };
            
            double res[BLOCK_SIZE][BLOCK_SIZE];

            for (int n = 0; n < BLOCK_SIZE; n++) 
            {
                for (int m = 0; m < BLOCK_SIZE; m++) 
                {
                    double dInputSum = 0;
                    
                    for (int i = 0; i < BLOCK_SIZE; i++) 
                    {
                        for (int j = 0; j < BLOCK_SIZE; j++) 
                        {
                            dInputSum += src(y, x)(i,j) *
                                cos(((2.0 * static_cast<double>(j) + 1.0) * static_cast<double>(m) * M_PIl) / (2.0 * static_cast<double>(BLOCK_SIZE))) *
                                cos(((2.0 * static_cast<double>(i) + 1.0) * static_cast<double>(n) * M_PIl) / (2.0 * static_cast<double>(BLOCK_SIZE)));
                        }
                    }
                    res[n][m] = (getCval(n, m) * dInputSum);
                }
            }

            for (int n = 0; n < BLOCK_SIZE; n++) 
            {
                for (int m = 0; m < BLOCK_SIZE; m++) 
                    dst(y, x)(n, m) = res[n][m];
            }
        };

    dst.Resize(src.row(), src.col());
    for (int n = 0; n < src.row(); n++)
    {
        for (int m = 0; m < src.col(); m++)
            dct(n, m);
    }
}

void DctTransformer::Quantization(BlockMatrix& src, BlockMatrix& dst)
{
    auto quantizate_block = [&](int y, int x)
        {
            for (int n = 0; n < BLOCK_SIZE; n++)
            {
                for (int m = 0; m < BLOCK_SIZE; m++)
                    dst(y, x)(n, m) = src(y, x)(n, m) / luminanceValues[n][m];
            }
        };

    dst.Resize(src.row(), src.col());
    for (int n = 0; n < src.row(); n++)
    {
        for (int m = 0; m < src.col(); m++)
            quantizate_block(n, m);
    }
}

void DctTransformer::InverseQuantization(BlockMatrix& src, BlockMatrix& dst)
{
    auto inverse_quantizate_block = [&](int y, int x)
        {
            for (int n = 0; n < BLOCK_SIZE; n++)
            {
                for (int m = 0; m < BLOCK_SIZE; m++)
                    dst(y, x)(n, m) = src(y, x)(n, m) * luminanceValues[n][m];
            }
        };

    dst.Resize(src.row(), src.col());
    for (int n = 0; n < src.row(); n++)
    {
        for (int m = 0; m < src.col(); m++)
            inverse_quantizate_block(n, m);
    }
}

void DctTransformer::IdctBlockVector(BlockMatrix& src, BlockMatrix& dst)
{
    {
        auto idct = [&](int y, int x)
            {
                //오픈 소스의 idct코드를 그대로 가져옴
                auto getCval = [](int y, int x) {
                    if (y == 0 && x == 0)
                        return 1.0 / double(BLOCK_SIZE);
                    else if (y != 0 && x != 0)
                        return 2.0 / double(BLOCK_SIZE);
                    else
                        return sqrt(2.0) / double(BLOCK_SIZE);
                    };

                double res[BLOCK_SIZE][BLOCK_SIZE];

                for (int n = 0; n < BLOCK_SIZE; n++) {
                    for (int m = 0; m < BLOCK_SIZE; m++) {
                        double dInputSum = 0;
                        for (int i = 0; i < BLOCK_SIZE; i++) {
                            for (int j = 0; j < BLOCK_SIZE; j++) {
                                dInputSum += getCval(i, j) * src(y, x)(i, j) *
                                    cos(((2.0 * static_cast<double>(m) + 1.0) * static_cast<double>(j) * M_PIl) / (2.0 * static_cast<double>(BLOCK_SIZE))) *
                                    cos(((2.0 * static_cast<double>(n) + 1.0) * static_cast<double>(i) * M_PIl) / (2.0 * static_cast<double>(BLOCK_SIZE)));
                            }
                        }
                        res[n][m] =  dInputSum;
                    }
                }

                for (int n = 0; n < BLOCK_SIZE; n++) {
                    for (int m = 0; m < BLOCK_SIZE; m++)
                        dst(y, x)(n, m) = res[n][m];
                }
            };

        dst.Resize(src.row(), src.col());
        for (int n = 0; n < src.row(); n++)
        {
            for (int m = 0; m < src.col(); m++)
                idct(n, m);
        }
    }
}

void DctTransformer::MergeBlock(BlockMatrix& block_data, cv::Mat& merged)
{
    auto copy_block_data = [&](int y, int x)
        {
            for (int n = 0; n < BLOCK_SIZE; n++)
            {
                for (int m = 0; m < BLOCK_SIZE; m++)
                {
                    int pix_idx = (n + y * BLOCK_SIZE) * merged.cols + m + x * BLOCK_SIZE;
                    merged.data[pix_idx] = block_data(y, x)(n, m);
                }
            }
        };

    int merged_rows = block_data.row() * BLOCK_SIZE;
    int merged_cols = block_data.col() * BLOCK_SIZE;
    merged.create(merged_rows, merged_cols, CV_8UC1);

    for (int n = 0; n < block_data.row(); n++)
    {
        for (int m = 0; m < block_data.col(); m++)
            copy_block_data(n, m);
    }
}

void DctTransformer::TransformImgToDct(cv::Mat& src, BlockMatrix& dst)
{
    cv::Mat padded;
    ResizeAndPad(src, padded);
    SplitImg(padded, dst);
    DctBlockVector(dst, dst);
    Quantization(dst, dst);
}

void DctTransformer::TransformDctToImg(BlockMatrix& src, cv::Mat& dst)
{
    BlockMatrix res;
    InverseQuantization(src, res);
    IdctBlockVector(res, res);
    MergeBlock(res, dst);
}
