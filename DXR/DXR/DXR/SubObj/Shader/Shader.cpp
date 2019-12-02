#include "Shader.h"
#include "../../Func/Func.h"
#include <d3d12.h>

// コンストラクタ
Shader::Shader(const std::string& fileName, const std::string& entry, const std::string& ver, const std::initializer_list<std::string>& func) :
	blob(nullptr), desc(std::make_unique<D3D12_DXIL_LIBRARY_DESC>())
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
	(*sub).pDesc = &(*desc);
	(*sub).Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;

	expo.resize(func.size());
	name.resize(func.size());

	desc->DXILLibrary.BytecodeLength  = blob->GetBufferSize();
	desc->DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
	desc->NumExports                  = unsigned int(func.size());
	desc->pExports                    = &(*expo[0]);

	unsigned int index = 0;
	for (auto& i : func)
	{
		name[index] = DXR::ChangeCode(i);

		expo[index]                 = std::make_unique<D3D12_EXPORT_DESC>();
		expo[index]->ExportToRename = nullptr;
		expo[index]->Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		expo[index]->Name           = name[index].data();
		++index;
	}
}

// シェーダブロブの取得
ID3DBlob* Shader::Get(void) const
{
	return blob;
}
