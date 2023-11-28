#pragma once
#include <memory>
#include <vector>
#include <array>
#include <algorithm>

#define BLOCK_SIZE 8
#define MAX_WARTER_MARK_LEN 1024

class Block
{
private:
	int height = BLOCK_SIZE;
	int width = BLOCK_SIZE;

	double* dct_res;

public:
	double& operator()(int y, int x);

	double& DC();

	double& LargestAC();

	int LargestACPos();

	static bool CompDC(Block& a, Block& b);

	static bool CompAC(Block& a, Block& b);

	Block();

	~Block();
};

class BlockMatrix
{
private:
	int rows=0;
	int cols=0;

	std::vector<std::vector<std::shared_ptr<Block>>> res;
	std::vector<std::shared_ptr<Block>> sorted;

public:
	int row();

	int col();

	Block& operator()(int y, int x);

	std::shared_ptr<Block> GetBlockPointer(int y, int x);

	void Resize(int rows, int cols);

	std::vector<std::shared_ptr<Block>>& SortBlock(std::vector<std::array<int,4>>& input_pos);
};