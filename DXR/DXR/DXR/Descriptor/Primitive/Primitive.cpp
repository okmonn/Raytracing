#include "Primitive.h"
#include <d3d12.h>
#include <crtdbg.h>

// コンストラクタ
Primitive::Primitive(const Vec3f* vertex, const size_t& vertexNum, const size_t& instanceNum) : 
	instanceNum(instanceNum)
{
	CreateVertexRsc(sizeof(vertex[0]) * vertexNum);
	
	void* buf = nullptr;
	UpData(&buf, vertex);
}

// デストラクタ
Primitive::~Primitive()
{
}

// 頂点リソースの生成
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

// 頂点データを更新する
void Primitive::UpData(void** buf, const Vec3f* vertex)
{
	Map(buf);
	std::memcpy(*buf, vertex, (*rsc.begin())->GetDesc().Width);
	UnMap();
}

// 移動
void Primitive::Translation(const size_t& instanceID, const Vec3f& vec)
{
	matrix[instanceID].Translation(vec);
}

// 頂点リソースの取得
ID3D12Resource* Primitive::Get(void) const
{
	return *rsc.begin();
}

// インスタンス数の取得
size_t Primitive::InstanceNum(void) const
{
	return instanceNum;
}

// 行列の取得
Matrix3x4 Primitive::Matrix(const size_t& instanceID) const
{
	return matrix[instanceID];
}
