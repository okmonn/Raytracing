#include "Primitive.h"
#include <d3d12.h>

// �R���X�g���N�^
Primitive::Primitive(const Vec3f* vertex, const size_t& vertexNum, const size_t& instanceNum)
{
	matrix.resize(instanceNum);

	CreateRsc(sizeof(vertex[0]) * vertexNum);
	UpData(vertex);
}

// �f�X�g���N�^
Primitive::~Primitive()
{
}

// ���\�[�X�̐���
void Primitive::CreateRsc(const size_t& size)
{
	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc = { 1, 0 };
	desc.Width = unsigned int(size);

	Descriptor::CreateRsc(UploadProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// ���_�f�[�^�̍X�V
void Primitive::UpData(const Vec3f* vertex)
{
	void* buf = nullptr;
	Map(&buf);
	std::memcpy(buf, vertex, (*rsc.begin())->GetDesc().Width);
	UnMap();
}

// �C���X�^���X���̎擾
size_t Primitive::InstanceNum(void) const
{
	return matrix.size();
}

// �s��̎擾
Matrix3x4 Primitive::Matrix(const size_t& index) const
{
	return matrix[index];
}
