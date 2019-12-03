#pragma once
#include "../Information.h"

struct ID3D12CommandAllocator;

class Allocator
{
public:
	// コンストラクタ
	Allocator(const DXR::CommandType& type);
	// デストラク
	~Allocator();

	// コマンドアロケータ
	void Reset(void) const;

	// コマンドアロケータの取得
	ID3D12CommandAllocator* Get(void) const;

private:
	// コマンドアロケータの生成
	void CreateAllo(const DXR::CommandType& type);


	// コマンドアロケータ
	ID3D12CommandAllocator* allo;
};
