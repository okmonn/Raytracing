#pragma once
#include "../SubObj.h"
#include <string>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

class Shader :
	public SubObj
{
public:
	// コンストラクタ
	Shader(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry = "");
	// デストラクタ
	~Shader();

	// シェーダブロブの取得
	ID3DBlob* Get(void) const;

	// コレクション数の取得
	size_t Num(void) const;

private:
	// サブオブジェクトの生成
	void CreateSub(const std::initializer_list<std::string>& func);


	// シェーダブロブ
	ID3DBlob* blob;

	// コレクション数
	size_t num;
};
