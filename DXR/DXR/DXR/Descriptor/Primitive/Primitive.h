#pragma once
#include "../Descriptor.h"
#include "../../Matrix.h"
#include <vector>

class Primitive :
	public Descriptor
{
public:
	// コンストラクタ
	Primitive(const Vec3f* vertex, const size_t& vertexNum, const size_t& instanceNum = 1);
	// デストラクタ
	~Primitive();

	// 移動
	void Translation(const size_t& instanceID, const Vec3f& vec);

	// 頂点リソースの取得
	ID3D12Resource* Get(void) const;

	// インスタンス数の取得
	size_t InstanceNum(void) const;
	
	// 行列の取得
	Matrix3x4 Matrix(const size_t& instanceID) const;

private:
	// 頂点リソースの生成
	void CreateVertexRsc(const size_t& size);

	// 頂点データを更新する
	void UpData(void** buf, const Vec3f* vertex);


	// インスタンス数
	size_t instanceNum;

	// 行列
	std::vector<Matrix3x4>matrix;
};
