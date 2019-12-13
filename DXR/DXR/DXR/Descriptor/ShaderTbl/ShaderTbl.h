#pragma once
#include "../Descriptor.h"
#include <string>

class Output;
class Pipe;

class ShaderTbl :
	public Descriptor
{
public:
	// コンストラクタ
	ShaderTbl(const Pipe* pipe);
	// デストラクタ
	~ShaderTbl();

	// レイジェネレーション用
	void RayGeneration(const std::string& name, const Output* output);

	// ミス用
	void Miss(const std::string& name);

	// クローゼストヒット用
	void Closest(const std::string& name);

	// シェーダエントリーサイズの取得
	static size_t EntrySize(void);

private:
	// リソースの生成
	void CreateRsc(const size_t& num);


	// パイプ
	Pipe* pipe;
};
