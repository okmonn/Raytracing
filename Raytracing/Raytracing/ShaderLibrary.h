#pragma once
#include <string>
#include <vector>
#include <d3d12.h>
#include <unordered_map>

struct IDxcBlob;

class ShaderLibrary
{
	struct Library
	{
		//ライブラリデスク
		D3D12_DXIL_LIBRARY_DESC library;
		//ステータスオブジェクト
		D3D12_STATE_SUBOBJECT state;
		//関数名
		std::vector<std::wstring>funcName;
		//関数情報
		std::vector<D3D12_EXPORT_DESC>desc;
		IDxcBlob* blob;

		Library() {
			library = {};
			state   = {};
		}
	};

public:
	// インスタンス変数取得
	static ShaderLibrary& Get(void);

	// シェーダコンパイル
	void Compile(const std::wstring& fileName, const std::initializer_list<std::wstring>& entry, const std::wstring& ver);

	// ステータスオブジェクト取得
	D3D12_STATE_SUBOBJECT GetSubObject(const std::wstring& fileName);

	// シェーダバイトコード取得
	D3D12_SHADER_BYTECODE GetByteCode(const std::wstring& fileName);

private:
	// コンストラクタ
	ShaderLibrary();
	// デストラク
	~ShaderLibrary();

	// シェーダコンパイル
	void Compile(const std::wstring& fileName, const std::wstring& ver, IDxcBlob** blob);


	// シェーダ情報
	std::unordered_map<std::wstring, Library>hlsl;
};
