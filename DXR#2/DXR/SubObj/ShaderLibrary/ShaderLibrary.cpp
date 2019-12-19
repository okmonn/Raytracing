#include "ShaderLibrary.h"
#include "../../Func/Func.h"
#include <d3d12.h>

// �R���X�g���N�^
ShaderLibrary::ShaderLibrary(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry) :
	blob(nullptr), lib(std::make_unique<D3D12_DXIL_LIBRARY_DESC>())
{
	DXR::Compile(fileName, entry, ver, &blob);
	SetSubObj(func);
}

// �f�X�g���N�^
ShaderLibrary::~ShaderLibrary()
{
	if (blob != nullptr)
	{
		blob->Release();
		blob = nullptr;
	}
}

// �T�u�I�u�W�F�N�g�̐ݒ�
void ShaderLibrary::SetSubObj(const std::initializer_list<std::string>& func)
{
	name.resize(func.size());
	expo.resize(func.size());
	unsigned int index = 0;
	for (auto& i : func)
	{
		name[index] = DXR::ChangeCode(i);

		expo[index].ExportToRename = nullptr;
		expo[index].Flags = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		expo[index].Name = name[index].data();

		++index;
	}

	(*lib).DXILLibrary.BytecodeLength = blob->GetBufferSize();
	(*lib).DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
	(*lib).NumExports = unsigned int(func.size());
	(*lib).pExports = expo.data();

	(*sub).pDesc = &(*lib);
	(*sub).Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
}

// �V�F�[�_�u���u�̎擾
ID3DBlob* ShaderLibrary::Blob(void) const
{
	return blob;
}
