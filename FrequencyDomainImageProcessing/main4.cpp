#include "Watermark.h"

using namespace std;
using namespace cv;

int main() {
    Mat src = imread("Lena512.jpg", IMREAD_GRAYSCALE);
    Mat dst;
    BlockMatrix dct_data;
    DctTransformer::TransformImgToDct(src, dct_data);

    //워터 마크 생성
    vector<double> randomSequence = generateRandomSequence();
    for (const auto& value : randomSequence) 
    {
        std::cout << value << " ";
    }
    cout << endl;
    //워터마크 삽입
    WatermarkInsert(dct_data, randomSequence);

    DctTransformer::TransformDctToImg(dct_data, dst);
    imshow("afterinsert", dst);

    //비교군 원본데이터
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

    //저장한 이미지 독립성분 분석
    vector<double>extractSequence;
    ExtractWatermark(src_data, afterdct_data, extractSequence);
    for (const auto& value : extractSequence) {
        std::cout << value << " ";
    }
    //유사도평가
    cout << NSProb(randomSequence, extractSequence) << "%";
    waitKey(0);

    return 0;
}
