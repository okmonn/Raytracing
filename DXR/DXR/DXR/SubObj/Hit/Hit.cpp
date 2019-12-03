#include "Hit.h"
#include "../../Func/Func.h"
#include <d3d12.h>

// �R���X�g���N�^
Hit::Hit(const std::string& hitName, const char* closest, const char* any) :
	hitName(DXR::ChangeCode(hitName))
{
	CreateSub(DXR::ChangeCode(closest), DXR::ChangeCode(any));
}

// �f�X�g���N�^
Hit::~Hit()
{
}

// �T�u�I�u�W�F�N�g�̐���
void Hit::CreateSub(const wchar_t* closest, const wchar_t* any)
{
	D3D12_HIT_GROUP_DESC desc{};

	desc.AnyHitShaderImport     = any;
	desc.ClosestHitShaderImport = closest;
	desc.HitGroupExport         = hitName.c_str();

	(*sub).pDesc = &desc;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
}
