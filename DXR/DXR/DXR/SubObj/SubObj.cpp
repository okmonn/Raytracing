#include "SubObj.h"
#include "../Func/Func.h"
#include <d3d12.h>

// コンストラクタ
SubObj::SubObj(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry) :
	sub(std::make_unique<D3D12_STATE_SUBOBJECT>()), blob(nullptr)
{
	DXR::Compile(fileName, entry, ver, &blob);
	SetSubObj(func);
}

// デストラクタ
SubObj::~SubObj()
{
	if (blob != nullptr)
	{
		blob->Release();
		blob = nullptr;
	}
}

// サブオブジェクトの設定
void SubObj::SetSubObj(const std::initializer_list<std::string>& func)
{
	std::vector<D3D12_EXPORT_DESC>expo(func.size());
	unsigned int index = 0;
	for (auto& i : func)
	{
		expo[index].ExportToRename = nullptr;
		expo[index].Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		expo[index].Name           = DXR::ChangeCode(i).data();

		++index;
	}

	D3D12_DXIL_LIBRARY_DESC desc{};
	desc.DXILLibrary.BytecodeLength  = blob->GetBufferSize();
	desc.DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
	desc.NumExports                  = unsigned int(func.size());
	desc.pExports                    = expo.data();

	(*sub).pDesc = &desc;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
}

// サブオブジェクトの取得
D3D12_STATE_SUBOBJECT SubObj::Get(void) const
{
	return *sub;
}

// シェーダブロブの取得
ID3DBlob* SubObj::Blob(void) const
{
	return blob;
}
