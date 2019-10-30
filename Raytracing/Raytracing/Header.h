#pragma once
#include <wrl.h>
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>

// �������J��
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// �E�B���h�E�T�C�YX
#define WINSIZE_X 640
// �E�B���h�E�T�C�YY
#define WINSIZE_Y 480

// �o�b�N�o�b�t�@�[�J�E���g
#define BACK_BUFFER 3

// �@�\���x���ꗗ
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
};

// �t�F���X
struct Fence
{
	//�t�F���X
	ID3D12Fence* ptr;
	//�J�E���g
	unsigned __int32 cnt;
	//�C�x���g
	void* event;

	Fence() {
		cnt   = 0;
		event = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	}
};

// �����_�[�^�[�Q�b�g
struct Render
{
	//�q�[�v
	ID3D12DescriptorHeap* heap;
	//���\�[�X
	std::vector<ID3D12Resource*>rsc;
};
