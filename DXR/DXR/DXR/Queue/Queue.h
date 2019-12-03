#pragma once
#include "../Information.h"

struct ID3D12CommandQueue;
struct ID3D12CommandList;

class Queue
{
public:
	// コンストラクタ
	Queue(const DXR::CommandType& type);
	// デストラク
	~Queue();

	// コマンドの実行
	void Execution(ID3D12CommandList* const* list, const size_t& num = 1);

	// コマンドキューの取得
	ID3D12CommandQueue* Get(void) const;

private:
	// コマンドキューの生成
	void CreateQueue(const DXR::CommandType& type);


	// コマンドキュー
	ID3D12CommandQueue* queue;
};
