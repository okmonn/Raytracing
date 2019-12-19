#include "Root.h"
#include "../ShaderLibrary/ShaderLibrary.h"
#include "../../Device/Device.h"
#include <wrl.h>
#include <array>
#include <d3d12.h>

// �R���X�g���N�^
Root::Root(const DXR::RootType& type, const DXR::RootInfo& info) :
	root(nullptr)
{
	CreateRoot(info);
	SetSubObj(type);
}

// �R���X�g���N�^
Root::Root(const ShaderLibrary* shaderLib) : 
	root(nullptr)
{
	CreateRoot(shaderLib);
}

// �f�X�g���N�^
Root::~Root()
{
	if (root != nullptr)
	{
		root->Release();
		root = nullptr;
	}
}

// ���C�W�F�l���[�V�����p���[�g�V�O�l�`�����
DXR::RootInfo Root::RayGenDesc(void)
{
	DXR::RootInfo info(2, 1);
	info.range[0].BaseShaderRegister = 0;
	info.range[0].NumDescriptors = 1;
	info.range[0].OffsetInDescriptorsFromTableStart = 0;
	info.range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	info.range[0].RegisterSpace = 0;
	//acceleration
	info.range[1].BaseShaderRegister = 0;
	info.range[1].NumDescriptors = 1;
	info.range[1].OffsetInDescriptorsFromTableStart = 1;
	info.range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	info.range[1].RegisterSpace = 0;

	info.param[0].DescriptorTable.NumDescriptorRanges = info.range.size();
	info.param[0].DescriptorTable.pDescriptorRanges = info.range.data();
	info.param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

	info.desc->Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	info.desc->NumParameters = info.param.size();
	info.desc->pParameters = info.param.data();

	return info;
}

// ���[�g�V�O�l�`���̐���
void Root::CreateRoot(const DXR::RootInfo& info)
{
	Microsoft::WRL::ComPtr<ID3DBlob>blob = nullptr;
	auto hr = D3D12SerializeRootSignature(&(*info.desc), D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &blob, nullptr);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);
}

// ���[�g�V�O�l�`���̐���
void Root::CreateRoot(const ShaderLibrary* shaderLib)
{
	auto hr = Device::Get()->CreateRootSignature(0, shaderLib->Blob()->GetBufferPointer(), shaderLib->Blob()->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);
}

// �T�u�I�u�W�F�N�g�̐ݒ�
void Root::SetSubObj(const DXR::RootType& type)
{
	sub->pDesc = &root;
	sub->Type = D3D12_STATE_SUBOBJECT_TYPE(type);
}

// ���[�g�V�O�l�`���̎擾
ID3D12RootSignature* Root::RtSig(void) const
{
	return root;
}
