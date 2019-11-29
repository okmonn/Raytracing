#pragma once
#include <string>
#include <vector>
#include <memory>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;
struct D3D12_DXIL_LIBRARY_DESC;
struct D3D12_STATE_SUBOBJECT;
struct D3D12_EXPORT_DESC;
struct ID3D12RootSignature;

class Root
{
public:
	/*コンストラクタ
	ローカルルートシグネチャ*/
	Root(const std::string& fileName, const std::string& entry, const std::string& ver, const std::initializer_list<std::string>& func);
	/*コンストラクタ
	グローバルルートシグネチャ*/
	Root();
	// デストラクタ
	~Root();

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const;

	// シェーダのサブオブジェクトの取得
	D3D12_STATE_SUBOBJECT ShaderSub(void) const;

	// ルートシグネチャのサブオブジェクトの取得
	D3D12_STATE_SUBOBJECT RootSub(void) const;

private:
	// シェーダのサブオブジェクトの生成
	void CreateShaderSub(const std::initializer_list<std::string>& func);

	// ローカルルートシグネチャの生成
	void CreateLocalRoot(void);

	// グローバルルートシグネチャの生成
	void CreateGlobalRoot(void);


	// ルートシグネチャ
	ID3D12RootSignature* root;

	// シェーダ情報
	ID3DBlob* shader;

	// DXILライブラリ
	std::unique_ptr<D3D12_DXIL_LIBRARY_DESC>desc;

	// サブオブジェクト
	std::vector<std::unique_ptr<D3D12_STATE_SUBOBJECT>>sub;

	// オブジェクトのエクスポート
	std::vector<std::unique_ptr<D3D12_EXPORT_DESC>>expo;

	// シェーダ関数名
	std::vector<std::wstring>name;
};
