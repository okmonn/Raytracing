#pragma once

struct ID3D12RootSignature;
class SubObj;

class Root
{
public:
	// コンストラクタ
	Root(const SubObj* sub);
	// デストラクタ
	~Root();

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const;

private:
	// ルートシグネチャの生成
	void CreateRoot(const SubObj* sub);


	// ルートシグネチャ
	ID3D12RootSignature* root;
};
