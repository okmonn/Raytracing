#pragma once
#include "../Descriptor.h"
#include <vector>
#include <memory>

struct D3D12_RAYTRACING_GEOMETRY_DESC;
struct D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS;
struct D3D12_RAYTRACING_INSTANCE_DESC;
class List;
class Primitive;

class Acceleration :
	public Descriptor
{
public:
	/*コンストラクタ
	ボトムレベル*/
	Acceleration(const List* list, const Primitive* prim);
	/*コンストラクタ
	トップレベル*/
	Acceleration(const List* list, const Acceleration* bottom, const size_t& bottomNum);
	// デストラクタ
	~Acceleration();

	// トップレベルの更新
	void UpData(const size_t& rayNum = 1);

	// リザルトリソースの取得
	ID3D12Resource* Result(void) const;

	// スクラッチリソースの取得
	ID3D12Resource* Scratch(void) const;

	// インスタンスリソースの取得
	ID3D12Resource* Instance(void) const;

	// 加速構造入力情報の取得
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS Input(void) const;

private:
	// ボトムレベルの生成
	void CreateBottom(void);

	// トップレベルの生成
	void CreateTop(const Acceleration* bottom, const size_t& bottomNum);

	// ビルド
	void Build(const List* list);


	// リスト
	const List* list;

	// プリミティブ
	const Primitive* prim;

	// バッファ
	D3D12_RAYTRACING_INSTANCE_DESC* buf;

	// ジオメトリ情報
	std::unique_ptr<D3D12_RAYTRACING_GEOMETRY_DESC>geo;

	// 加速構造入力情報
	std::unique_ptr<D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS>input;

	// ボトムレベル
	std::vector<const Acceleration*>bottom;
};
