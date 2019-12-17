#include "Swap.h"
#include "../Window/Window.h"
#include "../Queue/Queue.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

// �R���X�g���N�^
Swap::Swap(const Window* win, const Queue* queue, const size_t& bufferNum) :
	swap(nullptr)
{
	CreateSwap(win, queue, bufferNum);
}

// �f�X�g���N
Swap::~Swap()
{
	if (swap != nullptr)
	{
		swap->Release();
		swap = nullptr;
	}
}

// �X���b�v�`�F�C���̐���
void Swap::CreateSwap(const Window* win, const Queue* queue, const size_t& bufferNum)
{
	Microsoft::WRL::ComPtr<IDXGIFactory7>factory = nullptr;
	auto hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));
	_ASSERT(hr == S_OK);

	Vec2 winSize = win->WinSize();

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = unsigned int(bufferNum);
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = winSize.y;
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = winSize.x;

	hr = factory->CreateSwapChainForHwnd(queue->Get(), win->Get(), &desc, nullptr, nullptr, (IDXGISwapChain1**)&swap);
	_ASSERT(hr == S_OK);
}

// ��ʂ̍X�V
void Swap::Present(const unsigned int& sync, const unsigned int& flag)
{
	auto hr = swap->Present(sync, flag);
	_ASSERT(hr == S_OK);
}

// �X���b�v�`�F�C���̎擾
IDXGISwapChain4* Swap::Get(void) const
{
	return swap;
}

// �o�b�N�o�b�t�@���̎擾
unsigned int Swap::BufferNum(void) const
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	auto hr = swap->GetDesc1(&desc);
	_ASSERT(hr == S_OK);

	return desc.BufferCount;
}

// ���݂̃o�b�t�@�ԍ��̎擾
unsigned int Swap::Index(void) const
{
	return swap->GetCurrentBackBufferIndex();
}
