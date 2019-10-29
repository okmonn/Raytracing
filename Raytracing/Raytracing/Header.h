#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

// �������J��
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// �E�B���h�E�T�C�YX
#define WINSIZE_X 640
// �E�B���h�E�T�C�YY
#define WINSIZE_Y 480

// �@�\���x���ꗗ
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
};
