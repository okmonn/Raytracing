#pragma once
#include "../Descriptor.h"

class Swap;

class Render :
	public Descriptor
{
public:
	// コンストラクタ
	Render(const Swap* swap);
	// デストラクタ
	~Render();

private:
	// レンダーリソースの生成
	void CreateRenderRsc(const Swap* swap);
};
