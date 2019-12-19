#pragma once
#include "../Descriptor.h"
#include <string>

class Pipe;
class Output;

class ShaderTbl :
	public Descriptor
{
public:
	// コンストラクタ
	ShaderTbl(const Pipe* pipe);
	// デストラクタ
	~ShaderTbl();

	// レイジェネレーション
	void RayGeneration(const Output* output, const std::initializer_list<std::string>& name);

	// ミス
	void Miss(const std::initializer_list<std::string>& name);

	// クローゼストヒット
	void ClosestHit(const std::initializer_list<std::string>& name);

	// エントリーサイズの取得
	static size_t EntrySize(void);

private:
	// リソースの生成
	void CreateRsc(const size_t& entryNum);


	// パイプ
	const Pipe* pipe;
};
