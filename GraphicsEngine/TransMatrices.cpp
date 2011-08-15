#include "TransMatrices.h"

TransMatrices* TransMatrices::INSTANCE = 0;

TransMatrices::TransMatrices(void)
{
	std::stack<Mat4f> _matrixStacks;
}

TransMatrices::~TransMatrices(void)
{
	_matrixStacks.empty();
}

void TransMatrices::Push()
{
	_matrixStacks.push(modelView);
}

void TransMatrices::Pop()
{
	modelView = _matrixStacks.top();
	_matrixStacks.pop();
}