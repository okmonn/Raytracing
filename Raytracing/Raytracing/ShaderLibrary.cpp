#include "ShaderLibrary.h"

#pragma comment(lib, "dxcompiler.lib")

// コンストラクタ
ShaderLibrary::ShaderLibrary()
{
}

// デストラク
ShaderLibrary::~ShaderLibrary()
{
}

// コンパイル
void ShaderLibrary::Compile(const std::wstring& fileName, const std::wstring& entry, const std::wstring& ver, IDxcBlob** blob)
{
	Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcIncludeHandler>handler = nullptr;
	library->CreateIncludeHandler(&handler);
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
	library->CreateBlobFromFile(fileName.c_str(), nullptr, &encode);
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcCompiler>compiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
	hr = compiler->Compile(encode.Get(), fileName.c_str(), entry.c_str(), ver.c_str(), nullptr, 0, nullptr, 0, handler.Get(), &result);
	_ASSERT(hr == S_OK);

	result->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		hr = result->GetResult(&(*blob));
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

// コンパイル
void ShaderLibrary::Compile(const std::wstring& fileName, const std::wstring& entry, const std::initializer_list<std::wstring>& func, const std::wstring& ver)
{
	if (info.find(fileName) == info.end())
	{
		Compile(fileName, entry, ver, (IDxcBlob**)&info[fileName].blob);

		info[fileName].sub.pDesc = &info[fileName].desc;
		info[fileName].sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;

		info[fileName].expo.resize(func.size());
		info[fileName].name.resize(func.size());

		info[fileName].desc.DXILLibrary.BytecodeLength  = info[fileName].blob->GetBufferSize();
		info[fileName].desc.DXILLibrary.pShaderBytecode = info[fileName].blob->GetBufferPointer();
		info[fileName].desc.NumExports = func.size();
		info[fileName].desc.pExports   = info[fileName].expo.data();

		unsigned int index = 0;
		for (auto& i : func)
		{
			info[fileName].name[index] = i;

			info[fileName].expo[index].ExportToRename = nullptr;
			info[fileName].expo[index].Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
			info[fileName].expo[index].Name           = info[fileName].name[index].c_str();
			++index;
		}
	}
}

// シェーダ情報取得
ShaderInfo ShaderLibrary::GetInfo(const std::wstring& fileName)
{
	if (info.find(fileName) != info.end())
	{
		return info[fileName];
	}

	return ShaderInfo();
}

// インスタンス変数取得
ShaderLibrary& ShaderLibrary::Get(void)
{
	static ShaderLibrary instance;
	return instance;
}
