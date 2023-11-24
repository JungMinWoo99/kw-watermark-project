#include "DCTTransformer.h"
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main() {
    Mat src = imread("Lena512.jpg", IMREAD_GRAYSCALE);
    Mat dst;
    BlockMatrix dct_data;
    DctTransformer::TransformImgToDct(src, dct_data);

    //·Î°í »ðÀÔ
    Mat logo_img = imread("logo.jpg", IMREAD_GRAYSCALE);
    threshold(logo_img, logo_img, 128, 255, cv::THRESH_BINARY);
    resize(logo_img, logo_img, Size(dct_data.row(), dct_data.col()));
    imshow("logo", logo_img);

    for (int n = 0; n < dct_data.row(); n++)
    {
        for (int m = 0; m < dct_data.col(); m++)
        {
            if (logo_img.data[n * dct_data.col() + m] == 0)
            {
                for (int i = 0; i < BLOCK_SIZE; i++)
                {
                    for (int l = 0; l < BLOCK_SIZE; l++)
                    {
                        if (dct_data(n,m)(i,l) != 0)
                            dct_data(n,m)(i,l) += 10;
                    }
                }
                dct_data(n, m)(0, 0) -= 10;
            }
        }
    }

    DctTransformer::TransformDctToImg(dct_data, dst);

    imshow("hand", dst);

    waitKey(0);

    return 0;
}
