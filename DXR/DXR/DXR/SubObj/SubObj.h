#pragma once
#include <string>
#include <vector>
#include <memory>

struct D3D12_EXPORT_DESC;
struct D3D12_DXIL_LIBRARY_DESC;
struct D3D12_STATE_SUBOBJECT;
struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

class SubObj
{
public:
	// コンストラクタ
	SubObj(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry = "");
	// デストラクタ
	~SubObj();

	// サブオブジェクトの取得
	D3D12_STATE_SUBOBJECT Get(void) const;

	// シェーダブロブの取得
	ID3DBlob* Blob(void) const;

private:
	// サブオブジェクトの設定
	void SetSubObj(const std::initializer_list<std::string>& func);


	// エクスポート名
	std::vector<std::wstring>name;

	// エクスポート情報
	std::vector<D3D12_EXPORT_DESC>expo;

	// DXILライブラリ
	std::unique_ptr<D3D12_DXIL_LIBRARY_DESC>lib;

	// サブオブジェクト
	std::unique_ptr<D3D12_STATE_SUBOBJECT>sub;

	// シェーダブロブ
	ID3DBlob* blob;
};
