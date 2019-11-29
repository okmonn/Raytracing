#pragma once

struct IDXGISwapChain4;
class Window;
class Queue;

class Swap
{
public:
	// コンストラクタ
	Swap(const Window* win, const Queue* queue, const size_t& bufferNum = 2);
	// デストラク
	~Swap();

	// 画面の更新
	void Present(const unsigned int& sync = 0, const unsigned int& flag = 0);

	// スワップチェインの取得
	IDXGISwapChain4* Get(void) const;

	// バックバッファ数の取得
	unsigned int BufferNum(void) const;

	// 現在のバッファ番号の取得
	unsigned int Index(void) const;

private:
	// スワップチェインの生成
	void CreateSwap(const Window* win, const Queue* queue, const size_t& bufferNum);


	// スワップチェイン
	IDXGISwapChain4* swap;
};
