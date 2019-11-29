#pragma once

struct ID3D12Fence1;
class Queue;

class Fence
{
public:
	// コンストラクタ
	Fence(const Queue* queue);
	// デストラク
	~Fence();

	// 待機
	void Wait(void);

private:
	// フェンスの生成
	void CreateFence(void);


	// キュー
	const Queue* queue;

	// フェンス
	ID3D12Fence1* fence;

	// イベントハンドル
	void* handle;

	// フェンスカウント
	unsigned __int64 cnt;
};
