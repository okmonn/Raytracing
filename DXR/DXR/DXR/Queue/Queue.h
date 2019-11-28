#pragma once
#include "../Information.h"

struct ID3D12CommandQueue;

class Queue
{
public:
	// コンストラクタ
	Queue(const DXR::CommandType& type);
	// デストラク
	~Queue();

	// コマンドキューの取得
	ID3D12CommandQueue* Get(void);

private:
	// コマンドキューの生成
	void CreateQueue(const DXR::CommandType& type);


	// コマンドキュー
	ID3D12CommandQueue* queue;
};
