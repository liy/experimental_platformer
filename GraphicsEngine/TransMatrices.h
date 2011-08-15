#pragma once
#include <stack>
#include "acMath.h"

class TransMatrices
{
private:
	static TransMatrices* INSTANCE;

public:
	TransMatrices(void);
	~TransMatrices(void);

	static TransMatrices* Instance(){
		if(INSTANCE == 0){
			INSTANCE = new TransMatrices();
		}
		return INSTANCE;
	}

	Mat4f modelView;

	Mat4f projection;

	void Push();
	void Pop();

protected:
	std::stack<Mat4f> _matrixStacks;
};

