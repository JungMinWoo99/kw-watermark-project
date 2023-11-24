#include "DCTTransformer.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <random>
#include<math.h>
#include<algorithm>
using namespace std;
using namespace cv;
#define TH_DC 100.0
#define TH1 0.032
#define INSERTSIZE 2
float WEIGHT = 0.01;

//���������� ����
vector<float> generateRandomSequence() {
    const int length = 1000;
    const double mean = 5.0;
    const double stddev = 1.0;
    const double threshold = 0.01;

    // ���� ������ ���� ���� ������ �ʱ�ȭ
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> distribution(mean, stddev);

    // ���� ������ ����
    vector<float> randomSequence(length);
    for (int i = 0; i < length;) {
        float value = distribution(gen);
        if (abs(value) >= threshold) {
            randomSequence[i] = value;
            i++;
        }
    }

    // ���� ��, �� ���Ҹ� �Ҽ��� ��° �ڸ����� ����
    sort(randomSequence.begin(), randomSequence.end(), [](float a, float b) {
        return abs(a) < abs(b);
        });

    for (auto& value : randomSequence) {
        // �� ���Ҹ� �Ҽ��� ��° �ڸ����� �ݿø�
        value = round(value * 100.0) / 100.0;
    }

    return randomSequence;
}
//�����Լ� ����
float dotProduct(const vector<float>& array1, const vector<float>& array2) {
    if (array1.size() != array2.size() || array1.empty()) {
        cout << array1.size() << endl;
        cout << array2.size() << endl;
        cerr << "Error: Input arrays have different sizes or are empty." << std::endl;
        return 0.0; 
    }

    float result = 0.0;
    for (size_t i = 0; i < array1.size(); ++i) {
        result += array1[i] * array2[i];
    }

    return result;
}
//���͸�ũ ����
void WatermarkInsert(BlockMatrix& dct_data, vector<float>& randomSequence) {
    for (int n = 0; n < dct_data.row(); n++)
    {
        for (int m = 0; m < dct_data.col(); m++)
        {
            for (int h = 0; h < INSERTSIZE; h++) {
                for (int w = 0; w < INSERTSIZE; w++) {
                    //dc�� ��� ����
                    if (randomSequence.size() > 0) {
                        if (h == 0 && w == 0) {
                            float dctvalue = dct_data(n,m)(h, w);
                            if (dctvalue > TH_DC) {
                                int i = 0;
                                while (1) {
                                    if (abs(randomSequence[i] * WEIGHT) < TH1) {
                                        float value_log = randomSequence[i] * WEIGHT;
                                        randomSequence.erase(randomSequence.begin() + i);

                                        dct_data(n, m)(h, w) += value_log;
                                        break;
                                    }
                                    i++;
                                    if (i >= randomSequence.size())
                                        break;
                                }
                            }
                        }
                        if (h != 0 || w != 0) {

                            float weightvalue_log = randomSequence[randomSequence.size() - 1] * WEIGHT;
                            randomSequence.erase(randomSequence.begin() + randomSequence.size() - 1);


                            dct_data(n,m)(h, w) += weightvalue_log;
                        }
                    }
                }
            }
        }
    }
}
vector<float> ExtractWatermark( BlockMatrix& org,  BlockMatrix& dct_data, vector<float>& sequence) {
    int num = 0;
    for (int n = 0; n < dct_data.row(); n++)
    {
        for (int m = 0; m < dct_data.col(); m++)
        {
            for (int h = 0; h < INSERTSIZE; h++) {
                for (int w = 0; w < INSERTSIZE; w++) {
                    if (h == 0 && w == 0) {
                        float dctvalue = org(n,m)(h, w);
                        if (dctvalue > TH_DC) {
                            float value = dct_data(n, m)(h, w) - org(n, m)(h, w);
                            if (value != 0) {
                                float dstvalue = value - WEIGHT;
                                if (dstvalue == 0) {
                                    cout << "err";
                                }
                                sequence.push_back(dstvalue * -1);
                            }
                        }
                    }
                    if (h != 0 || w != 0) {
                        float value = dct_data(n, m)(h, w) - org(n, m)(h, w);
                        if (value != 0) {
                            float dstvalue = value / (WEIGHT);
                            if (dstvalue == 0) {
                                cout << "err";
                            }
                            sequence.push_back(dstvalue * -1);
                        }
                    }
                }
            }
        }
        sort(sequence.begin(), sequence.end(), [](float a, float b) {
            return abs(a) < abs(b);
            });
    }
    for (auto& value : sequence) {
        // �� ���Ҹ� �Ҽ��� ��° �ڸ� ������ ������
        value = static_cast<float>(static_cast<int>(value * 100.0)) / 100.0;
    }
    return sequence;
}
float NSProb(const vector<float>& org, const vector<float>& sequence) {
    float orgvalue = dotProduct(org, org);
    float orgsequence = dotProduct(org, sequence);
    float sequencevalue = dotProduct(sequence, sequence);
    float NS = ((orgsequence / sqrt(sequencevalue)) / (orgvalue / sqrt(orgvalue))) * 100;
    return NS;
}
int main() {
    Mat src = imread("Lena512.jpg", IMREAD_GRAYSCALE);
    Mat dst;
    BlockMatrix dct_data;
    DctTransformer::TransformImgToDct(src, dct_data);
    for (int i = 0; i < dct_data.row(); i++)
    {
        for (int l = 0; l < dct_data.col(); l++)
        {
            cout << dct_data(i,l)(0,0)<< endl;
        }
    }


    //���� ��ũ ����
    vector<float> randomSequence = generateRandomSequence();
    vector<float> randomSequence2(randomSequence.begin(), randomSequence.end());
    
    //���͸�ũ ����
    WatermarkInsert(dct_data, randomSequence);

    DctTransformer::TransformDctToImg(dct_data, dst);
    BlockMatrix src_data;
    DctTransformer::TransformImgToDct(src, src_data);

    imshow("afterinsert", dst);

    //imwrite("test.jpg", dst);

    //Mat insertimg = imread("test.jpg", IMREAD_GRAYSCALE);

    //������ �̹��� �������� �м�
    //Mat afterdst;
    //BlockMatrix afterdct_data;
    //DctTransformer::TransformImgToDct(insertimg, afterdct_data);

    vector<float>extractSequence;

    ExtractWatermark(dct_data, src_data, extractSequence);

    //���絵��
    cout << NSProb(randomSequence2, extractSequence) << "%";
    waitKey(0);

    return 0;
}
