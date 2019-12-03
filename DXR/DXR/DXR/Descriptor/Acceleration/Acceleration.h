#pragma once
#include "../Descriptor.h"
#include <vector>

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

private:
	// ボトムレベルの生成
	void CreateBottom(void);

	// トップレベルの生成
	void CreateTop(const Acceleration* bottom, const size_t& bottomNum);

	// ビルド
	void Build(const List* list, const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input);


	// リスト
	const List* list;

	// プリミティブ
	const Primitive* prim;

	// バッファ
	D3D12_RAYTRACING_INSTANCE_DESC* buf;

	// ボトムレベル
	std::vector<const Acceleration*>bottom;
};
