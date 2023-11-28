#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

vector<vector<int>> binarizeImage(const Mat& inputImage) {
    vector<vector<int>> binaryResult;

    if (inputImage.empty()) {
        cerr << "이미지를 불러올 수 없습니다." << endl;
        return binaryResult;
    }

    Mat grayImage;
    cvtColor(inputImage, grayImage, COLOR_BGR2GRAY);

    int rows = grayImage.rows;
    int cols = grayImage.cols;

    binaryResult.resize(rows, vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grayImage.at<uchar>(i, j) < 128) {
                // 검은색인 경우
                binaryResult[i][j] = -1;
            }
            else {
                // 흰색 또는 다른 색상인 경우
                binaryResult[i][j] = 1;
            }
        }
    }

    return binaryResult;
}

Mat visualizeBinaryImage(const vector<vector<int>>& binaryResult) {
    int rows = binaryResult.size();
    int cols = binaryResult[0].size();

    Mat binaryImage(rows, cols, CV_8UC1, Scalar(255));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (binaryResult[i][j] == -1) {
                // 검은색인 경우
                binaryImage.at<uchar>(i, j) = 0;
            }
            else {
                // 흰색 또는 다른 색상인 경우
                binaryImage.at<uchar>(i, j) = 255;
            }
        }
    }

    return binaryImage;
}

int main() {
    // 이미지 파일 경로 설정
    string imagePath = "logo.png";  // 본인의 이미지 경로로 변경

    // 이미지 불러오기
    Mat inputImage = imread(imagePath);

    // 이진화된 결과 얻기
    vector<vector<int>> binaryResult = binarizeImage(inputImage);

    // 결과 출력
    for (const auto& row : binaryResult) {
        cout << '{';
        for (int pixelValue : row) {
            cout << pixelValue << ", ";
        }
        cout << "}," << endl;
    }

    // 결과를 이진 영상으로 시각화하여 출력
    Mat binaryImage = visualizeBinaryImage(binaryResult);

    // 시각화된 결과 영상 출력
    imshow("Binary Image", binaryImage);
    waitKey(0);

    return 0;
}
