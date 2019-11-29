#include "Primitive.h"
#include <d3d12.h>
#include <crtdbg.h>

// �R���X�g���N�^
Primitive::Primitive(const Vec3f* vertex, const size_t& vertexNum, const size_t& instanceNum) : 
	instanceNum(instanceNum)
{
	CreateVertexRsc(sizeof(vertex[0]) * vertexNum);
	
	void* buf = nullptr;
	UpData(&buf, vertex);
}

// �f�X�g���N�^
Primitive::~Primitive()
{
}

// ���_���\�[�X�̐���
void Primitive::CreateVertexRsc(const size_t& size)
{
	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = unsigned int(size);

	CreateRsc(UploadProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// ���_�f�[�^���X�V����
void Primitive::UpData(void** buf, const Vec3f* vertex)
{
	Map(buf);
	std::memcpy(*buf, vertex, (*rsc.begin())->GetDesc().Width);
	UnMap();
}

// �ړ�
void Primitive::Translation(const size_t& instanceID, const Vec3f& vec)
{
	matrix[instanceID].Translation(vec);
}

// ���_���\�[�X�̎擾
ID3D12Resource* Primitive::Get(void) const
{
	return *rsc.begin();
}

// �C���X�^���X���̎擾
size_t Primitive::InstanceNum(void) const
{
	return instanceNum;
}

// �s��̎擾
Matrix3x4 Primitive::Matrix(const size_t& instanceID) const
{
	return matrix[instanceID];
}
