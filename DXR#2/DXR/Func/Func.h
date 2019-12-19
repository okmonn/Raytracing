#pragma once
#include <string>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

namespace DXR
{
	// マルチバイト文字からユニコード文字に変換
	std::wstring ChangeCode(const std::string& str);
	// マルチバイト文字からユニコード文字に変換
	const wchar_t* ChangeCode(const char* str);

	// マルチバイト文字の配列からユニコード文字の配列に変換
	const wchar_t** ChangeCode(const char** str, const size_t& num);

	// シェーダのコンパイル
	void Compile(const std::string& fileName, const std::string& entry, const std::string& ver, ID3DBlob** blob);
}
