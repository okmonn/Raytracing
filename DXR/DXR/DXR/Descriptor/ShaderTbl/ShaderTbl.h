#pragma once
#include "../Descriptor.h"

class Pipe;

class ShaderTbl :
	public Descriptor
{
public:
	// コンストラクタ
	ShaderTbl(const Pipe* pipe);
	// デストラクタ
	~ShaderTbl();

private:
	// シェーダテーブルサイズ
};
