#pragma once
#include <memory>

struct D3D12_STATE_SUBOBJECT;

class SubObj
{
public:
	// コンストラクタ
	SubObj();
	// デストラクタ
	virtual ~SubObj();

	// サブオブジェクトの取得
	D3D12_STATE_SUBOBJECT& Get(void) const;

protected:
	// サブオブジェクト
	std::unique_ptr<D3D12_STATE_SUBOBJECT>sub;
};
