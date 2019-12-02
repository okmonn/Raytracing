#include "Func.h"
#include <wrl.h>
#include <vector>
#include <dxcapi.h>
#include <Windows.h>

#pragma comment(lib, "dxcompiler.lib")

// マルチバイト文字からユニコード文字に変換
std::wstring DXR::ChangeCode(const std::string& str)
{
	std::vector<wchar_t>buf(MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &buf.front(), int(buf.size()));

	return std::wstring(buf.begin(), buf.end());
}

// マルチバイト文字からユニコード文字に変換
wchar_t* DXR::ChangeCode(const char* str)
{
	if (str == nullptr)
	{
		return nullptr;
	}
	
	wchar_t* tmp = nullptr;
	std::mbstowcs(tmp, str, std::strlen(str) + 1);

	return tmp;
}

// シェーダのコンパイル
void DXR::Compile(const std::string& fileName, const std::string& entry, const std::string& ver, ID3DBlob** blob)
{
	Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcIncludeHandler>handle = nullptr;
	hr = library->CreateIncludeHandler(&handle);
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
	hr = library->CreateBlobFromFile(ChangeCode(fileName).c_str(), nullptr, &encode);
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcCompiler>compile = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compile));
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
	hr = compile->Compile(encode.Get(), ChangeCode(fileName).c_str(), ChangeCode(entry).c_str(), ChangeCode(ver).c_str(), nullptr, 0, nullptr, 0, handle.Get(), &result);
	_ASSERT(hr == S_OK);

	result->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		hr = result->GetResult((IDxcBlob**)&(*blob));
		_ASSERT(hr == S_OK);
	}
	else
	{
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print   = nullptr;
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print16 = nullptr;

		hr = result->GetErrorBuffer(&print);
		_ASSERT(hr == S_OK);

		hr = library->GetBlobAsUtf16(print.Get(), &print16);
		_ASSERT(hr == S_OK);

		wprintf(L"%*s", (int)print16->GetBufferSize() / 2, (LPCWSTR)print16->GetBufferPointer());
	}
}
