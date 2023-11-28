#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

vector<vector<int>> binarizeImage(const Mat& inputImage) {
    vector<vector<int>> binaryResult;

    if (inputImage.empty()) {
        cerr << "�̹����� �ҷ��� �� �����ϴ�." << endl;
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
                // �������� ���
                binaryResult[i][j] = -1;
            }
            else {
                // ��� �Ǵ� �ٸ� ������ ���
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
                // �������� ���
                binaryImage.at<uchar>(i, j) = 0;
            }
            else {
                // ��� �Ǵ� �ٸ� ������ ���
                binaryImage.at<uchar>(i, j) = 255;
            }
        }
    }

    return binaryImage;
}

int main() {
    // �̹��� ���� ��� ����
    string imagePath = "logo.png";  // ������ �̹��� ��η� ����

    // �̹��� �ҷ�����
    Mat inputImage = imread(imagePath);

    // ����ȭ�� ��� ���
    vector<vector<int>> binaryResult = binarizeImage(inputImage);

    // ��� ���
    for (const auto& row : binaryResult) {
        cout << '{';
        for (int pixelValue : row) {
            cout << pixelValue << ", ";
        }
        cout << "}," << endl;
    }

    // ����� ���� �������� �ð�ȭ�Ͽ� ���
    Mat binaryImage = visualizeBinaryImage(binaryResult);

    // �ð�ȭ�� ��� ���� ���
    imshow("Binary Image", binaryImage);
    waitKey(0);

    return 0;
}
