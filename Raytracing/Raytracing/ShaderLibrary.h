#pragma once
#include "Header.h"
#include <dxcapi.h>
#include <unordered_map>

class ShaderLibrary
{
public:
	// コンパイル
	void Compile(const std::wstring& fileName, const std::wstring& entry, const std::initializer_list<std::wstring>& func, const std::wstring& ver);

	// シェーダ情報取得
	ShaderInfo GetInfo(const std::wstring& fileName);

	// インスタンス変数取得
	static ShaderLibrary& Get(void);

private:
	ShaderLibrary(const ShaderLibrary&) = delete;
	void operator=(const ShaderLibrary&) = delete;

	// コンストラクタ
	ShaderLibrary();
	// デストラク
	~ShaderLibrary();

	// コンパイル
	void Compile(const std::wstring& fileName, const std::wstring& entry, const std::wstring& ver, IDxcBlob** blob);

	
	// シェーダ情報
	std::unordered_map<std::wstring, ShaderInfo>info;
};
