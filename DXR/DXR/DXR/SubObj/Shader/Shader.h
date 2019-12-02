#pragma once
#include "../SubObj.h"
#include <string>
#include <vector>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;
struct D3D12_DXIL_LIBRARY_DESC;
struct D3D12_EXPORT_DESC;

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

	// DXILライブラリ
	std::unique_ptr<D3D12_DXIL_LIBRARY_DESC>desc;

	// オブジェクトのエクスポート
	std::vector<std::unique_ptr<D3D12_EXPORT_DESC>>expo;

	// シェーダ関数名
	std::vector<std::wstring>name;
};
