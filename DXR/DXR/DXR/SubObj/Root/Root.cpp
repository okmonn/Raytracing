#include "Root.h"
#include "../../Device/Device.h"
#include "../Shader/Shader.h"
#include <d3d12.h>

// �R���X�g���N�^
Root::Root(const Shader* shader) : 
	root(nullptr)
{
	CreateRoot(shader);
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

// ���[�g�V�O�l�`���̐���
void Root::CreateRoot(const Shader* shader)
{
	auto hr = Device::Get()->CreateRootSignature(0, shader->Get()->GetBufferPointer(), shader->Get()->GetBufferSize(), IID_PPV_ARGS(&root));
	_ASSERT(hr == S_OK);
}

// ���[�g�V�O�l�`���̎擾
ID3D12RootSignature* Root::Get(void) const
{
	return root;
}
