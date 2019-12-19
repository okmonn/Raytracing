#pragma once
#include "../SubObj.h"
#include <string>
#include <vector>

struct D3D12_EXPORT_DESC;
struct D3D12_DXIL_LIBRARY_DESC;
struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

class ShaderLibrary :
	public SubObj
{
public:
	// コンストラクタ
	ShaderLibrary(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry = "");
	// デストラクタ
	~ShaderLibrary();

	// シェーダブロブの取得
	ID3DBlob* Blob(void) const;

private:
	// サブオブジェクトの設定
	void SetSubObj(const std::initializer_list<std::string>& func);


	// シェーダブロブ
	ID3DBlob* blob;

	// DXILライブラリ
	std::unique_ptr<D3D12_DXIL_LIBRARY_DESC>lib;

	// エクスポート名
	std::vector<std::wstring>name;

	// エクスポート情報
	std::vector<D3D12_EXPORT_DESC>expo;
};
