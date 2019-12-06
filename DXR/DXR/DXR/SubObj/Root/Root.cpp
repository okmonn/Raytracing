#include "Root.h"
#include "../../Device/Device.h"
#include "../Shader/Shader.h"
#include <wrl.h>
#include <vector>
#include <d3d12.h>

// コンストラクタ
Root::Root(const D3D12_ROOT_SIGNATURE_DESC& desc) :
	root(nullptr)
{
	CreateLocal(desc);
}

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

// レイジェネレーション用構造体の取得
D3D12_ROOT_SIGNATURE_DESC Root::RayGenDesc(void)
{
	std::vector<D3D12_DESCRIPTOR_RANGE>range(2);
	//output
	range[0].BaseShaderRegister                = 0;
	range[0].NumDescriptors                    = 1;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_APPEND_ALIGNED_ELEMENT;
	range[0].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	range[0].RegisterSpace                     = 0;
	//scene
	range[1].BaseShaderRegister                = 0;
	range[1].NumDescriptors                    = 1;
	range[1].OffsetInDescriptorsFromTableStart = D3D12_APPEND_ALIGNED_ELEMENT;
	range[1].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[1].RegisterSpace                     = 0;

	D3D12_ROOT_PARAMETER param{};
	param.DescriptorTable.NumDescriptorRanges = unsigned int(range.size());
	param.DescriptorTable.pDescriptorRanges   = range.data();
	param.ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags         = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	desc.NumParameters = 1;
	desc.pParameters   = &param;

	return desc;
}

// ミス用構造体の取得
D3D12_ROOT_SIGNATURE_DESC Root::MissDesc(void)
{
	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;

	return desc;
}

// ローカルルートシグネチャの生成
void Root::CreateLocal(const D3D12_ROOT_SIGNATURE_DESC& desc)
{
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;

	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &sig, nullptr);
	_ASSERT(hr == S_OK);

	hr = Device::Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);

	(*sub).pDesc = &root;
	(*sub).Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
}

// ローカルルートシグネチャの生成
void Root::CreateLocal(const Shader* shader)
{
	auto hr = Device::Get()->CreateRootSignature(0, shader->Get()->GetBufferPointer(), shader->Get()->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);
}

// グローバルルートシグネチャの生成
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

// ルートシグネチャの取得
ID3D12RootSignature* Root::Get(void) const
{
	return root;
}

