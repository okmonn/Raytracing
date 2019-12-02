#include "SubObj.h"
#include <d3d12.h>

// コンストラクタ
SubObj::SubObj() : 
	sub(std::make_unique<D3D12_STATE_SUBOBJECT>())
{
}

// デストラクタ
SubObj::~SubObj()
{
}

// サブオブジェクトの取得
D3D12_STATE_SUBOBJECT SubObj::Sub(void)
{
	return *sub;
}
