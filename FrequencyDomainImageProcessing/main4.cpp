#include "Watermark.h"

using namespace std;
using namespace cv;

int main() {
    Mat src = imread("Lena512.jpg", IMREAD_GRAYSCALE);
    Mat dst;
    BlockMatrix dct_data;
    DctTransformer::TransformImgToDct(src, dct_data);

    //���� ��ũ ����
    vector<double> randomSequence = generateRandomSequence();
    for (const auto& value : randomSequence) 
    {
        std::cout << value << " ";
    }
    cout << endl;
    //���͸�ũ ����
    WatermarkInsert(dct_data, randomSequence);

    DctTransformer::TransformDctToImg(dct_data, dst);
    imshow("afterinsert", dst);

    //�񱳱� ����������
    BlockMatrix src_data;
    DctTransformer::TransformImgToDct(src, src_data);

    for (int n = 0; n < 8; n++)
    {
        for (int m = 0; m < 8; m++)
        {
            cout << std::fixed << std::setprecision(2)<< src_data(0, 0)(n, m) << ' ';
        }
        cout<<endl;
    }

    BlockMatrix afterdct_data;
    DctTransformer::TransformImgToDct(dst, afterdct_data);

    //������ �̹��� �������� �м�
    vector<double>extractSequence;
    ExtractWatermark(src_data, afterdct_data, extractSequence);
    for (const auto& value : extractSequence) {
        std::cout << value << " ";
    }
    //���絵��
    cout << NSProb(randomSequence, extractSequence) << "%";
    waitKey(0);

    return 0;
}
