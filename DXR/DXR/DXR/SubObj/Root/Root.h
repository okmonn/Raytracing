#pragma once
#include "../SubObj.h"

struct ID3D12RootSignature;
class Shader;

class Root :
	public SubObj
{
public:
	// コンストラクタ
	Root(const Shader* shader);
	// デストラクタ
	~Root();

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const;

private:
	// ルートシグネチャの生成
	void CreateRoot(const Shader* shader);


	// ルートシグネチャ
	ID3D12RootSignature* root;
};
