#pragma once

struct ID3D12Fence1;
class Queue;

class Fence
{
public:
	// コンストラクタ
	Fence(Queue* queue);
	// デストラク
	~Fence();

	// 待機
	void Wait(void);

private:
	// フェンスの生成
	void CreateFence(void);


	// キュー
	Queue* queue;

	// フェンス
	ID3D12Fence1* fence;

	// イベントハンドル
	void* handle;

	// フェンスカウント
	unsigned __int64 cnt;
};
