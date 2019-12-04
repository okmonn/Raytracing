#include "Root.h"
#include "../../Device/Device.h"
#include "../Shader/Shader.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxcapi.h>

// �R���X�g���N�^
Root::Root(const Shader* shader) :
	root(nullptr)
{
	if (shader != nullptr)
	{
		CreateLocal(shader);
	}
	else
	{
		CreateLocal();
	}
}

// �R���X�g���N�^
Root::Root() : 
	root(nullptr)
{
	CreateGlobal();
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

// ���[�J�����[�g�V�O�l�`���̐���
void Root::CreateLocal(void)
{
	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;

	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;

	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &sig, nullptr);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub).pDesc = &root;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
}

// ���[�J�����[�g�V�O�l�`���̐���
void Root::CreateLocal(const Shader* shader)
{
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;

	auto hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub).pDesc = &root;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
}

// �O���[�o�����[�g�V�O�l�`���̐���
void Root::CreateGlobal(void)
{
	D3D12_ROOT_SIGNATURE_DESC desc{};
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;

	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &sig, nullptr);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub).pDesc = &root;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
}

// ���[�g�V�O�l�`���̎擾
ID3D12RootSignature* Root::Get(void) const
{
	return root;
}

