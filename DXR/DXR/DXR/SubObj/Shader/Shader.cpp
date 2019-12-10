#include "Shader.h"
#include "../../Func/Func.h"
#include <vector>
#include <d3d12.h>

// コンストラクタ
Shader::Shader(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry) :
	blob(nullptr), num(func.size())
{
	DXR::Compile(fileName, entry, ver, &blob);
	CreateSub(func);
}

// デストラクタ
Shader::~Shader()
{
	if (blob != nullptr)
	{
		blob->Release();
		blob = nullptr;
	}
}

// サブオブジェクトの生成
void Shader::CreateSub(const std::initializer_list<std::string>& func)
{
	static std::vector<D3D12_EXPORT_DESC>expo(func.size());
	static std::vector<std::wstring>name(func.size());
	unsigned int index = 0;

	for (auto& i : func)
	{
		name[index] = DXR::ChangeCode(i);

		expo[index].ExportToRename = nullptr;
		expo[index].Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		expo[index].Name           = name[index].data();
		++index;
	}

	static D3D12_DXIL_LIBRARY_DESC lib{};
	lib.DXILLibrary.BytecodeLength  = blob->GetBufferSize();
	lib.DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
	lib.NumExports                  = unsigned int(func.size());
	lib.pExports                    = expo.data();

	(*sub).pDesc = &lib;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
}

// シェーダブロブの取得
ID3DBlob* Shader::Get(void) const
{
	return blob;
}

// コレクション数の取得
size_t Shader::Num(void) const
{
	return num;
}
