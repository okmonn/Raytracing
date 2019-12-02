#include "Hit.h"
#include "../../Func/Func.h"
#include <d3d12.h>

// コンストラクタ
Hit::Hit(const std::string& hitName, const char* closest, const char* any) :
	group(std::make_unique<D3D12_HIT_GROUP_DESC>()), hitName(DXR::ChangeCode(hitName))
{
	CreateSub(DXR::ChangeCode(closest), DXR::ChangeCode(any));
}

// デストラクタ
Hit::~Hit()
{
}

// サブオブジェクトの生成
void Hit::CreateSub(const wchar_t* closest, const wchar_t* any)
{
	(*group).AnyHitShaderImport     = any;
	(*group).ClosestHitShaderImport = closest;
	(*group).HitGroupExport         = hitName.c_str();

	(*sub).pDesc = &(*group);
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
}
