#pragma once
#include "../Vector.h"

struct HWND__;

class Window
{
public:
	// コンストラクタ
	Window(const Vec2& winSize);
	// デストラク
	~Window();

	// ウィンドウハンドルの取得
	HWND__* Get(void) const;

	// ウィンドウサイズの取得
	Vec2 WinSize(void) const;

	// メッセージの確認
	static bool CheckMsg(void);

private:
	// ウィンドウ生成
	void CreateWin(const Vec2& winSize);
	

	// ウィンドウハンドル
	void* handle;
};
