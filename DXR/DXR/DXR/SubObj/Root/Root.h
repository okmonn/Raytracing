#pragma once
#include "../SubObj.h"

struct D3D12_ROOT_SIGNATURE_DESC;
struct ID3D12RootSignature;
class Shader;

class Root :
	public SubObj
{
public:
	/*コンストラクタ
	ローカルルートシグネチャ*/
	Root(const D3D12_ROOT_SIGNATURE_DESC& desc);
	/*コンストラクタ
	グローバルルートシグネチャ*/
	Root();
	// デストラクタ
	~Root();

	// レイジェネレーション用構造体の取得
	static D3D12_ROOT_SIGNATURE_DESC RayGenDesc(void);

	// ミス用構造体の取得
	static D3D12_ROOT_SIGNATURE_DESC MissDesc(void);

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const;

private:
	// ローカルルートシグネチャの生成
	void CreateLocal(const D3D12_ROOT_SIGNATURE_DESC& desc);

	// グローバルルートシグネチャの生成
	void CreateGlobal(void);


	// ルートシグネチャ
	ID3D12RootSignature* root;
};
