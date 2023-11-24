#include "BlockMatrix.h"

using namespace std;

double& Block::operator()(int y, int x)
{
	return dct_res[y * width + x];
}

double& Block::DC()
{
	return dct_res[0];
}

double& Block::LargestAC()
{
	int max_idx = 0;
	for (int n = 0; n < height; n++)
	{
		for (int m = 0; m < width; m++)
		{
			if (abs((*this)(n, m)) > abs(dct_res[max_idx]))
				max_idx = n * width + m;
		}
	}
	return dct_res[max_idx];
}

int Block::LargestACPos()
{
	int max_idx = 0;
	for (int n = 0; n < height; n++)
	{
		for (int m = 0; m < width; m++)
		{
			if (abs((*this)(n, m)) > abs(dct_res[max_idx]))
				max_idx = n * width + m;
		}
	}
	return max_idx;
}

bool Block::CompDC(Block& a, Block& b)
{
	return a.DC() > b.DC();
}

bool Block::CompAC(Block& a, Block& b)
{
	return a.LargestAC() > b.LargestAC();
}

Block::Block()
{
	dct_res = new double[BLOCK_SIZE * BLOCK_SIZE];
}

Block::~Block()
{
	delete[] dct_res;
}

int BlockMatrix::row()
{
	return rows;
}

int BlockMatrix::col()
{
	return cols;
}

Block& BlockMatrix::operator()(int y, int x)
{
	if (y >= 0 && y < rows && x >= 0 && x < cols)
	{
		if (res[y][x] == nullptr)
			res[y][x] = std::make_shared<Block>();
		return *res[y][x].get();
	}
	else
		std::exit(EXIT_FAILURE);
}

std::shared_ptr<Block> BlockMatrix::GetBlockPointer(int y, int x)
{
	if (y >= 0 && y < rows && x >= 0 && x < cols)
		return res[y][x];
	else
		std::exit(EXIT_FAILURE);
}

void BlockMatrix::Resize(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;

	res.resize(rows);
	for (int i = 0; i < rows; i++)
		res[i].resize(cols);
}

std::vector<std::shared_ptr<Block>>& BlockMatrix::SortBlock(vector<array<int, 4>>& sorted_block_pos)
{
	sorted.clear();
	for (int n = 0; n < res.size(); n++)
	{
		for (int m = 0; m < res.front().size(); m++)
			sorted.push_back(res[n][m]);
	}

	auto comp_dc = [](std::shared_ptr<Block> a, std::shared_ptr<Block> b)
		{
			return Block::CompDC(*a.get(), *b.get());
		};
	//std::sort(sorted.begin(), sorted.end(), comp_dc);

	auto comp_ac = [](std::shared_ptr<Block> a, std::shared_ptr<Block> b)
		{
			return Block::CompAC(*a.get(), *b.get());
		};
	std::sort(sorted.begin(), sorted.end(), comp_ac);

	sorted_block_pos.resize(MAX_WARTER_MARK_LEN);
	for (int i = 0; i < MAX_WARTER_MARK_LEN; i++)
	{
		for (int n = 0; n < this->row(); n++)
		{
			for (int m = 0; m < this->col(); m++)
			{
				if (sorted[i] == this->GetBlockPointer(n, m))
				{
					if (i < MAX_WARTER_MARK_LEN / 2)
					{
						sorted_block_pos[i][0] = n;
						sorted_block_pos[i][1] = m;
						sorted_block_pos[i][2] = 0;
						sorted_block_pos[i][3] = 0;
					}
					else
					{
						sorted_block_pos[i][0] = n;
						sorted_block_pos[i][1] = m;
						int ac_idx = (*this)(n, m).LargestACPos();
						sorted_block_pos[i][2] = ac_idx / BLOCK_SIZE;
						sorted_block_pos[i][3] = ac_idx % BLOCK_SIZE;
					}
				}
			}
		}
	}

	return sorted;
}
