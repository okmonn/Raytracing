#include "Root.h"
#include "../Device/Device.h"
#include "../Func/Func.h"
#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

// ���[�J�����[�g�V�O�l�`���̃T�u�I�u�W�F�N�g��
#define LOCAL_SUB_NUM 2
// �O���[�o�����[�g�V�O�l�`���̃T�u�I�u�W�F�N�g��
#define GLOBAL_SUB_NUM 1

// �R���X�g���N�^
Root::Root(const std::string& fileName, const std::string& entry, const std::string& ver, const std::initializer_list<std::string>& func) : 
	root(nullptr), shader(nullptr), desc(std::make_unique<D3D12_DXIL_LIBRARY_DESC>())
{
	sub.resize(LOCAL_SUB_NUM);
	for (auto& i : sub)
	{
		i = std::make_unique<D3D12_STATE_SUBOBJECT>();
	}

	DXR::Compile(fileName, entry, ver, &shader);
	CreateShaderSub(func);

	CreateLocalRoot();
}

// �R���X�g���N�^
Root::Root() : 
	root(nullptr), shader(nullptr), desc(nullptr)
{
	sub.resize(GLOBAL_SUB_NUM);

	CreateGlobalRoot();
}

// �f�X�g���N�^
Root::~Root()
{
	if (root != nullptr)
	{
		root->Release();
		root = nullptr;
	}
	if (shader != nullptr)
	{
		shader->Release();
		shader = nullptr;
	}
}

// �V�F�[�_�̃T�u�I�u�W�F�N�g�̐���
void Root::CreateShaderSub(const std::initializer_list<std::string>& func)
{
	(*sub.begin())->pDesc = &(*desc);
	(*sub.begin())->Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;

	expo.resize(func.size());
	name.resize(func.size());

	desc->DXILLibrary.BytecodeLength  = shader->GetBufferSize();
	desc->DXILLibrary.pShaderBytecode = shader->GetBufferPointer();
	desc->NumExports                  = unsigned int(func.size());
	desc->pExports                    = &(*expo[0]);

	unsigned int index = 0;
	for (auto& i : func)
	{
		name[index] = DXR::ChangeCode(i);

		expo[index] = std::make_unique<D3D12_EXPORT_DESC>();
		expo[index]->ExportToRename = nullptr;
		expo[index]->Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		expo[index]->Name           = name[index].data();
		++index;
	}
}

// ���[�J�����[�g�V�O�l�`���̐���
void Root::CreateLocalRoot(void)
{
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
	auto hr = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(), D3D_BLOB_PART::D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub.rbegin())->pDesc = &root;
	(*sub.rbegin())->Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
}

// �O���[�o�����[�g�V�O�l�`���̐���
void Root::CreateGlobalRoot(void)
{
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
	D3D12_ROOT_SIGNATURE_DESC desc{};

	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &sig, nullptr);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub.rbegin())->pDesc = &root;
	(*sub.rbegin())->Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
}

// ���[�g�V�O�l�`���̎擾
ID3D12RootSignature* Root::Get(void) const
{
	return root;
}

// �V�F�[�_�̃T�u�I�u�W�F�N�g�̎擾
D3D12_STATE_SUBOBJECT Root::ShaderSub(void) const
{
	return *(*sub.begin());
}

// ���[�g�V�O�l�`���̃T�u�I�u�W�F�N�g�̎擾
D3D12_STATE_SUBOBJECT Root::RootSub(void) const
{
	return *(*sub.rbegin());
}
