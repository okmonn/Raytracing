#pragma once
#include <string>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

namespace DXR
{
	// マルチバイト文字からユニコード文字に変換
	std::wstring ChangeCode(const std::string& str);
	// マルチバイト文字からユニコード文字に変換
	wchar_t* ChangeCode(const char* str);

	// シェーダのコンパイル
	void Compile(const std::string& fileName, const std::string& entry, const std::string& ver, ID3DBlob** blob);
}
