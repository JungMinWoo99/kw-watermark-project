#include "DCTTransformer.h"
#include <opencv2/opencv.hpp>
#include <vector>

#define BLOCK_SIZE 8
using namespace std;
using namespace cv;

//색상 이미지 처리시 필요한 휘도를 양자화 하는 테이블
   /*
   vector<vector<int>> chrominanceValues = {
       {17, 18, 24, 47, 99, 99, 99, 99},
       {18, 21, 26, 66, 99, 99, 99, 99},
       {24, 26, 56, 99, 99, 99, 99, 99},
       {47, 66, 99, 99, 99, 99, 99, 99},
       {99, 99, 99, 99, 99, 99, 99, 99},
       {99, 99, 99, 99, 99, 99, 99, 99},
       {99, 99, 99, 99, 99, 99, 99, 99},
       {99, 99, 99, 99, 99, 99, 99, 99}
   };
   */

void ResizeAndPad(Mat& inputImage,Mat& dst)
{
    // 입력 이미지의 크기를 2의 제곱수로 조정합니다.
    int optimal_rows = getOptimalDFTSize(inputImage.rows);
    int optimal_cols = getOptimalDFTSize(inputImage.cols);

    // 크기를 조정한 이미지를 새로 생성합니다.
    copyMakeBorder(inputImage, dst, 0, optimal_rows - inputImage.rows, 0, optimal_cols - inputImage.cols, BORDER_CONSTANT, Scalar::all(0));
    imshow("padded", dst);
}

void SplitImg(Mat& padded, vector<vector<Mat>>& img_block_vector)
{
    // 이미지를 8*8 블록단위로 분할
    int rows_block_num = padded.rows / BLOCK_SIZE;
    int cols_block_num = padded.cols / BLOCK_SIZE;

    img_block_vector.resize(rows_block_num);
    for (int n = 0; n < rows_block_num; n++)
    {
        img_block_vector[n].resize(cols_block_num);
        for (int m = 0; m < cols_block_num; m++)
        {
            Mat blocked(padded, Rect(BLOCK_SIZE * n, BLOCK_SIZE * m, BLOCK_SIZE, BLOCK_SIZE));
            blocked.convertTo(blocked, CV_32FC1);//dct를 하기위해 실수형 자료형으로
            img_block_vector[n][m] = blocked;
        }
    }
}

void DctBlockVector(vector<vector<Mat>>& img_block_vector, vector<vector<Mat>>& dct_block_vector)
{
    dct_block_vector.resize(img_block_vector.size());
    for (int n = 0; n < img_block_vector.size(); n++)
    {
        dct_block_vector[n].resize(img_block_vector[n].size());
        for (int m = 0; m < img_block_vector[n].size(); m++)
            dct(img_block_vector[n][m], dct_block_vector[n][m]);
    }
}

void Quantization(vector<vector<Mat>>& dct_block_vector)
{
    //밝기를 양자화 하는 테이블
    vector<vector<int>> luminanceValues = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68, 109, 103, 77},
        {24, 35, 55, 64, 81, 104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}
    };
    Mat quantizationTable(8, 8, CV_32F);
    for (int i = 0; i < quantizationTable.rows; ++i) {
        for (int j = 0; j < quantizationTable.cols; ++j) {
            quantizationTable.at<float>(i, j) = (float)luminanceValues[i][j];
        }
    }
   
    for (int n = 0; n < dct_block_vector.size(); n++)
    {
        for (int m = 0; m < dct_block_vector[n].size(); m++)
        {
            divide(dct_block_vector[n][m], quantizationTable, dct_block_vector[n][m]);
            dct_block_vector[n][m].convertTo(dct_block_vector[n][m], CV_32S);
        }
    }
}

void InverseQuantization(vector<vector<Mat>>& dct_block_vector)
{
    //밝기를 양자화 하는 테이블
    vector<vector<int>> luminanceValues = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68, 109, 103, 77},
        {24, 35, 55, 64, 81, 104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}
    };
    Mat quantizationTable(8, 8, CV_32F);
    for (int i = 0; i < quantizationTable.rows; ++i) {
        for (int j = 0; j < quantizationTable.cols; ++j) {
            quantizationTable.at<float>(i, j) = (float)luminanceValues[i][j];
        }
    }
    
    for (int n = 0; n < dct_block_vector.size(); n++)
    {
        for (int m = 0; m < dct_block_vector[n].size(); m++)
        {
            dct_block_vector[n][m].convertTo(dct_block_vector[n][m], CV_32F);
            multiply(dct_block_vector[n][m], quantizationTable, dct_block_vector[n][m]);
        }
    }
}

void IdctBlockVector(vector<vector<Mat>>& dct_block_vector, vector<vector<Mat>>& idct_block_vector)
{
    idct_block_vector.resize(dct_block_vector.size());
    for (int n = 0; n < dct_block_vector.size(); n++)
    {
        idct_block_vector[n].resize(dct_block_vector[n].size());
        for (int m = 0; m < dct_block_vector[n].size(); m++)
            dct(dct_block_vector[n][m], idct_block_vector[n][m], DCT_INVERSE);
    }
}

void MergeBlock(vector<vector<Mat>>& idct_block_vector, Mat& merged_img)
{
    for (int n = 0; n < idct_block_vector.size(); n++)
    {
        for (int m = 0; m < idct_block_vector[n].size(); m++)
        {
            idct_block_vector[n][m].convertTo(idct_block_vector[n][m], CV_8UC1);

            Rect roi(BLOCK_SIZE * n, BLOCK_SIZE * m, BLOCK_SIZE, BLOCK_SIZE);
            Mat roiRegion = merged_img(roi);
            idct_block_vector[n][m].copyTo(roiRegion);
        }
    }
    imshow("res", merged_img);
}

int main() {
    // 이미지를 로드합니다.
    Mat inputImage = imread("Lena512.jpg", IMREAD_GRAYSCALE);
    imshow("inputImage", inputImage);

   
    // 크기를 조정한 이미지를 새로 생성합니다.
    Mat padded;
    ResizeAndPad(inputImage, padded);

    // 이미지를 8*8 블록단위로 분할
    vector<vector<Mat>> img_block_vector;
    SplitImg(padded, img_block_vector);


    // 각각의 블록에 대해 이진 코사인 변환
    vector<vector<Mat>> dct_block_vector;
    DctBlockVector(img_block_vector, dct_block_vector);


    //일반적으로 사용되는 양자화 테이블을 바탕으로 각각의 블록을 양자화
    Quantization(dct_block_vector);

    
    /*//dct 블록들을 보여주는 임시 코드
    Mat tem(padded.size(), dct_block_vector[0][0].type());
    for (int n = 0; n < dct_block_vector.size(); n++)
    {
        for (int m = 0; m < dct_block_vector[n].size(); m++)
        {
            Rect roi(BLOCK_SIZE * n, BLOCK_SIZE * m, BLOCK_SIZE, BLOCK_SIZE);
            Mat roiRegion = tem(roi);
            dct_block_vector[n][m].copyTo(roiRegion);
        }
    }
    imshow("tem",tem);
    waitKey(0);
    */

    //임시 로고 삽입
    /*
    * 로고를 block_vector 크기로 바꾸고
    * 로고가 검은색인 좌표의 블록 전체중 0이 아닌 계수에 1씩 더하기
    */
    //로고 이진화 및 축소
    Mat logo_img = imread("logo.jpg", IMREAD_GRAYSCALE);
    threshold(logo_img, logo_img, 128, 255, cv::THRESH_BINARY);
    resize(logo_img, logo_img,Size(dct_block_vector.size(), dct_block_vector[0].size()));
    imshow("logo", logo_img);

    /*
    for (int n = 0; n < dct_block_vector.size(); n++)
    {
        for (int m = 0; m < dct_block_vector[n].size(); m++)
        {
            if (logo_img.data[m * dct_block_vector[n].size() + n] == 0)
            {
                for (int i = 0; i < BLOCK_SIZE; i++)
                {
                    for (int l = 0; l < BLOCK_SIZE; l++)
                    {
                        if (dct_block_vector[n][m].at<int>(i, l) != 0)
                            dct_block_vector[n][m].at<int>(i, l) += 10;
                    }
                }
                dct_block_vector[n][m].at<int>(0, 0) -= 10;
            }
        }
    }
    */
   
    //역 양자화
    InverseQuantization(dct_block_vector);

    //역 DCT
    vector<vector<Mat>> idct_block_vector;
    IdctBlockVector(dct_block_vector, idct_block_vector);
    
    // 블록들을 하나로 합치기
    Mat merged_img(padded.size(), padded.type());
    MergeBlock(idct_block_vector, merged_img);


    //직접만든 클래스
    Mat src = imread("Lena512.jpg", IMREAD_GRAYSCALE);
    Mat dst;
    BlockMatrix dct_data;
    DctTransformer::TransformImgToDct(src, dct_data);
    DctTransformer::TransformDctToImg(dct_data, dst);

    imshow("hand", dst);

    waitKey(0);

    return 0;
}
