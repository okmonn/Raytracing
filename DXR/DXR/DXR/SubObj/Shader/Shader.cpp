#include "Shader.h"
#include "../../Func/Func.h"
#include <vector>
#include <d3d12.h>

// コンストラクタ
Shader::Shader(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry) :
	blob(nullptr)
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
	D3D12_DXIL_LIBRARY_DESC lib{};

	(*sub).pDesc = &lib;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;

	std::vector<D3D12_EXPORT_DESC>expo(func.size());
	std::vector<std::wstring>name(func.size());

	lib.DXILLibrary.BytecodeLength  = blob->GetBufferSize();
	lib.DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
	lib.NumExports                  = unsigned int(func.size());
	lib.pExports                    = expo.data();

	unsigned int index = 0;
	for (auto& i : func)
	{
		name[index] = DXR::ChangeCode(i);

		expo[index].ExportToRename = nullptr;
		expo[index].Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		expo[index].Name           = name[index].data();
		++index;
	}
}

// シェーダブロブの取得
ID3DBlob* Shader::Get(void) const
{
	return blob;
}
