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
	Shader(const std::string& fileName, const std::string& entry, const std::string& ver, const std::initializer_list<std::string>& func);
	// デストラクタ
	~Shader();

	// シェーダブロブの取得
	ID3DBlob* Get(void) const;

private:
	// サブオブジェクトの生成
	void CreateSub(const std::initializer_list<std::string>& func);


	// シェーダブロブ
	ID3DBlob* blob;
};
