#include "Root.h"
#include "../../Device/Device.h"
#include "../Shader/Shader.h"
#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

// コンストラクタ
Root::Root(const Shader* shader) :
	root(nullptr)
{
	CreateLocal(shader);
}

// コンストラクタ
Root::Root() : 
	root(nullptr)
{
	CreateGlobal();
}

// デストラクタ
Root::~Root()
{
	if (root != nullptr)
	{
		root->Release();
		root = nullptr;
	}
}

// ローカルルートシグネチャの生成
void Root::CreateLocal(const Shader* shader)
{
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
	auto hr = D3DGetBlobPart(shader->Get()->GetBufferPointer(), shader->Get()->GetBufferSize(), D3D_BLOB_PART::D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub).pDesc = &root;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
}

// グローバルルートシグネチャの生成
void Root::CreateGlobal(void)
{
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
	D3D12_ROOT_SIGNATURE_DESC desc{};

	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &sig, nullptr);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub).pDesc = &root;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
}

// ルートシグネチャの取得
ID3D12RootSignature* Root::Get(void) const
{
	return root;
}

