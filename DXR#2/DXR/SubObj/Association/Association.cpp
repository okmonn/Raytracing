#include "Association.h"
#include "../../Func/Func.h"
#include <d3d12.h>

// コンストラクタ
Association::Association(const SubObj* obj, const std::initializer_list<const wchar_t*>& exportName) :
	asso(std::make_unique<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION>())
{
	SetSubObj(obj, exportName);
}

// デストラクタ
Association::~Association()
{
}

// サブオブジェクトの設定
void Association::SetSubObj(const SubObj* obj, const std::initializer_list<const wchar_t*>& exportName)
{
	name.resize(exportName.size());
	size_t index = 0;
	for (auto& i : exportName)
	{
		name[index++] = i;
	}

	asso->NumExports = name.size();
	asso->pExports = name.data();
	asso->pSubobjectToAssociate = &obj->Get();

	sub->pDesc = &(*asso);
	sub->Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
}
