#pragma once
#include "../Descriptor.h"

class List;
class Swap;

class Render :
	public Descriptor
{
public:
	// コンストラクタ
	Render(const Swap* swap);
	// デストラクタ
	~Render();

	// クリア
	void Clear(const List* list, const float* color, ID3D12DescriptorHeap* depth = nullptr);

	// リソースの取得
	ID3D12Resource* Rsc(void) const;

private:
	// レンダーリソースの生成
	void CreateRenderRsc(void);

	// RTVの生成
	void RTV(const size_t& index);


	// スワップ
	const Swap* swap;
};
