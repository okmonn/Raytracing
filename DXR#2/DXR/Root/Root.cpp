#include "Root.h"
#include "../Device/Device.h"
#include "../SubObj/SubObj.h"
#include <d3d12.h>

// コンストラクタ
Root::Root(const SubObj* sub) :
	root(nullptr)
{
	CreateRoot(sub);
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

// ルートシグネチャの生成
void Root::CreateRoot(const SubObj* sub)
{
	auto hr = Device::Get()->CreateRootSignature(0, sub->Blob()->GetBufferPointer(), sub->Blob()->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);
}

// ルートシグネチャの取得
ID3D12RootSignature* Root::Get(void) const
{
	return root;
}
