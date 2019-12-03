#include "Pipe.h"
#include "../Device/Device.h"
#include "../SubObj/SubObj.h"
#include <vector>
#include <d3d12.h>

// �R���X�g���N�^
Pipe::Pipe(const std::initializer_list<SubObj*>& obj) :
	pipe(nullptr)
{
	CreatePipe(obj);
}

// �f�X�g���N�^
Pipe::~Pipe()
{
	if (pipe != nullptr)
	{
		pipe->Release();
		pipe = nullptr;
	}
}

// �p�C�v���C���̐���
void Pipe::CreatePipe(const std::initializer_list<SubObj*>& obj)
{
	std::vector<D3D12_STATE_SUBOBJECT>sub;
	for (auto& i : obj)
	{
		sub.push_back(i->Sub());
	}

	D3D12_STATE_OBJECT_DESC desc{};
	desc.NumSubobjects = unsigned int(sub.size());
	desc.pSubobjects   = sub.data();
	desc.Type          = D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;

	auto hr = Device::Get()->CreateStateObject(&desc, IID_PPV_ARGS(&pipe));
	_ASSERT(hr == S_OK);
}

// �p�C�v���C���̎擾
ID3D12StateObject* Pipe::Get(void) const
{
	return pipe;
}
