#pragma once
#include "../Descriptor.h"

struct D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS;
class List;
class Primitive;

class Acceleration :
	public Descriptor
{
public:
	// コンストラクタ
	Acceleration(const List* list, const Primitive* prim);
	// コンストラクタ
	Acceleration(const List* list, const std::initializer_list<Acceleration*>& bottom);
	// デストラクタ
	~Acceleration();

	// データのセット
	void UpData(const size_t& depth = 1);

	// スクラッチリソースの取得
	ID3D12Resource* Scratch(void) const;

	// リザルトリソースの取得
	ID3D12Resource* Result(void) const;

	// インスタンスリソースの取得
	ID3D12Resource* Instance(void) const;

private:
	// ボトムレベルの生成
	void CreateBottom(void);

	// トップレベルの生成
	void CreateTop();

	//	ビルド
	void Build(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input);


	// リスト
	const List* list;

	// プリミティブ
	const Primitive* prim;

	// ボトム
	std::vector<Acceleration*>bottom;
};
