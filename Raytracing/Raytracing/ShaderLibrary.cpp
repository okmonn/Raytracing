#include "ShaderLibrary.h"
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

// インスタンス変数取得
ShaderLibrary& ShaderLibrary::Get(void)
{
	static ShaderLibrary instance;
	return instance;
}

// コンストラクタ
ShaderLibrary::ShaderLibrary()
{
}

// デストラク
ShaderLibrary::~ShaderLibrary()
{
}

// シェーダコンパイル
void ShaderLibrary::Compile(const std::wstring& fileName, const std::wstring& ver, IDxcBlob** blob)
{
	Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	_ASSERT(hr == S_OK);
	Microsoft::WRL::ComPtr<IDxcIncludeHandler>handler = nullptr;
	hr = library->CreateIncludeHandler(&handler);
	_ASSERT(hr == S_OK);
	Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
	library->CreateBlobFromFile(fileName.c_str(), nullptr, &encode);
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcCompiler>compiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	_ASSERT(hr == S_OK);
	Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
	hr = compiler->Compile(encode.Get(), fileName.c_str(), L"", ver.c_str(), nullptr, 0, nullptr, 0, handler.Get(), &result);
	_ASSERT(hr == S_OK);

	result->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		hr = result->GetResult(blob);
		_ASSERT(hr == S_OK);
	}
	else
	{
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print  = nullptr;
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print16 = nullptr;

		hr = result->GetErrorBuffer(&print);
		_ASSERT(hr == S_OK);

		hr = library->GetBlobAsUtf16(print.Get(), &print16);
		_ASSERT(hr == S_OK);

		wprintf(L"%*s", (int)print16->GetBufferSize() / 2, (LPCWSTR)print16->GetBufferPointer());
	}
}

// シェーダコンパイル
void ShaderLibrary::Compile(const std::wstring& fileName, const std::initializer_list<std::wstring>& entry, const std::wstring& ver)
{
	if (hlsl.find(fileName) == hlsl.end())
	{
		Microsoft::WRL::ComPtr<IDxcBlob>blob = nullptr;
		Compile(fileName, ver, &blob);

		hlsl[fileName].funcName.resize(entry.size());
		hlsl[fileName].desc.resize(entry.size());
		unsigned int index = 0;
		for (auto itr = entry.begin(); itr != entry.end(); ++itr)
		{
			hlsl[fileName].funcName[index]            = *itr;
			hlsl[fileName].desc[index].ExportToRename = nullptr;
			hlsl[fileName].desc[index].Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
			hlsl[fileName].desc[index].Name           = hlsl[fileName].funcName[index].c_str();
			++index;
		}

		hlsl[fileName].library.DXILLibrary.BytecodeLength  = blob->GetBufferSize();
		hlsl[fileName].library.DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
		hlsl[fileName].library.NumExports                  = hlsl[fileName].desc.size();
		hlsl[fileName].library.pExports                    = hlsl[fileName].desc.data();
		
		hlsl[fileName].state.pDesc = &hlsl[fileName].desc;
		hlsl[fileName].state.Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
	}
}

// ステータスオブジェクト取得
D3D12_STATE_SUBOBJECT ShaderLibrary::GetSubObject(const std::wstring& fileName)
{
	if (hlsl.find(fileName) != hlsl.end())
	{
		return hlsl[fileName].state;
	}

	return D3D12_STATE_SUBOBJECT();
}

// シェーダバイトコード取得
D3D12_SHADER_BYTECODE ShaderLibrary::GetByteCode(const std::wstring& fileName)
{
	if (hlsl.find(fileName) != hlsl.end())
	{
		return hlsl[fileName].library.DXILLibrary;
	}

	return D3D12_SHADER_BYTECODE();
}
