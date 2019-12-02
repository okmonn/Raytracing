#pragma once
#include <memory>

struct D3D12_STATE_SUBOBJECT;

class SubObj
{
public:
	// サブオブジェクトの取得
	D3D12_STATE_SUBOBJECT Sub(void);

private:

protected:
	// コンストラクタ
	SubObj();
	// デストラクタ
	virtual ~SubObj();


	// サブオブジェクト
	std::unique_ptr<D3D12_STATE_SUBOBJECT>sub;
};
