#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

// メモリ開放
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// ウィンドウサイズX
#define WINSIZE_X 640
// ウィンドウサイズY
#define WINSIZE_Y 480

// 機能レベル一覧
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
};
