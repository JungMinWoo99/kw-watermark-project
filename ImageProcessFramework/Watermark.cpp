#include "pch.h"
#include "Watermark.h"
#include <iostream>
#include <random>
#include<math.h>
#include<algorithm>

#define WEIGHT 0.9

using namespace std;

vector<double> generateRandomSequence()
{
	const int length = MAX_WARTER_MARK_LEN;
	const double mean = 0;
	const double stddev = 1.0;
	const double threshold = 0.01;

	// 랜덤 엔진과 정규 분포 생성기 초기화
	random_device rd;
	mt19937 gen(rd());
	normal_distribution<double> distribution(mean, stddev);

	// 랜덤 시퀀스 생성
	vector<double> randomSequence(length);
	for (int i = 0; i < length;) {
		double value = distribution(gen);
		if (abs(value) >= threshold) {
			randomSequence[i] = value;
			i++;
		}
	}

	for (auto& value : randomSequence) {
		//각 원소를 소수점 둘째 자리까지 반올림
		value = round(value * 100.0) / 100.0;
	}

	return randomSequence;
}

void WatermarkInsert(BlockMatrix& dct_data, vector<double>& randomSequence)
{
	vector<array<int, 4>> sorted_block_pos;
	vector<shared_ptr<Block>>& sorted = dct_data.SortBlock(sorted_block_pos);
	for (int i = 0; i < sorted_block_pos.size(); i++)
	{
		array<int, 4> pos = sorted_block_pos[i];
		dct_data(pos[0],pos[1])(pos[2],pos[3]) += randomSequence[i] * WEIGHT;
	}
}

void ExtractWatermark(BlockMatrix& org, BlockMatrix& dct_data, vector<double>& extracted_sequence)
{
	BlockMatrix delta;
	delta.Resize(org.row(), org.col());

	for (int n = 0; n < dct_data.row(); n++)
	{
		for (int m = 0; m < dct_data.col(); m++)
		{
			for (int h = 0; h < BLOCK_SIZE; h++) 
			{
				for (int w = 0; w < BLOCK_SIZE; w++) 
				{
					delta(n, m)(h, w) = dct_data(n, m)(h, w) - org(n, m)(h, w);
				}
			}
		}
	}

	extracted_sequence.clear();
	vector<array<int, 4>> sorted_block_pos;
	vector<shared_ptr<Block>>& sorted = org.SortBlock(sorted_block_pos);
	for (int i = 0; i < sorted_block_pos.size(); i++)
	{
		array<int, 4> pos = sorted_block_pos[i];
		extracted_sequence.push_back(delta(pos[0], pos[1])(pos[2], pos[3]) / WEIGHT);
	}
	//reverse(extracted_sequence.begin(), extracted_sequence.begin()+ MAX_WARTER_MARK_LEN / 2);
	//reverse(extracted_sequence.begin() + MAX_WARTER_MARK_LEN / 2, extracted_sequence.end());

	for (auto& value : extracted_sequence) {
		// 각 원소를 소수점 둘째 자리 밑으로 버리기
		value = static_cast<double>(static_cast<int>(value * 100.0)) / 100.0;
	}
}

double NSProb(const vector<double>& org, const vector<double>& sequence)
{
	auto dotProduct = [](const vector<double>& array1, const vector<double>& array2) {
		if (array1.size() != array2.size() || array1.empty()) {
			cout << array1.size() << endl;
			cout << array2.size() << endl;
			cerr << "Error: Input arrays have different sizes or are empty." << std::endl;
			return 0.0;
		}

		double result = 0.0;
		for (size_t i = 0; i < array1.size(); ++i) {
			result += array1[i] * array2[i];
		}

		return result;
		};
	double orgvalue = dotProduct(org, org);
	double orgsequence = dotProduct(org, sequence);
	double sequencevalue = dotProduct(sequence, sequence);
	double NS = ((orgsequence / sqrt(sequencevalue)) / (orgvalue / sqrt(orgvalue))) * 100;
	return NS;
}