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

	// インスタンス数の取得
	size_t InstanceNum(void) const;

	// 行列の取得
	Matrix3x4 Matrix(const size_t& index) const;

private:
	// リソースの生成
	void CreateRsc(const size_t& size);

	// 頂点データの更新
	void UpData(const Vec3f* vertex);


	// 行列
	std::vector<Matrix3x4>matrix;
};
