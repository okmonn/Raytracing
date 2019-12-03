#pragma once
#include "../SubObj.h"

class PipeConfig :
	public SubObj
{
public:
	// コンストラクタ
	PipeConfig(const size_t& depth);
	// デストラクタ
	~PipeConfig();

private:
	// サブオブジェクトの生成
	void CreateSub(const size_t& depth);
};
