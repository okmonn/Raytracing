#pragma once
#include "../SubObj.h"

struct ID3D12RootSignature;
class Shader;

class Root :
	public SubObj
{
public:
	/*コンストラクタ
	ローカルルートシグネチャ*/
	Root(const Shader* shader);
	/*コンストラクタ
	グローバルルートシグネチャ*/
	Root();
	// デストラクタ
	~Root();

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const;

private:
	// ローカルルートシグネチャの生成
	void CreateLocal(void);

	// ローカルルートシグネチャの生成
	void CreateLocal(const Shader* shader);

	// グローバルルートシグネチャの生成
	void CreateGlobal(void);


	// ルートシグネチャ
	ID3D12RootSignature* root;
};
