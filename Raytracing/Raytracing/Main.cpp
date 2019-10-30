#include "Header.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// �E�B���h�E�R�[���o�b�N
#ifdef _WIN64
__int64 __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
#else
long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned int wParam, long lParam)
#endif
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(HWND(hWnd));
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(HWND(hWnd), message, wParam, lParam);
}

// ���b�Z�[�W�̊m�F
bool CheckMsg(void)
{
	static MSG msg{};
	static const wchar_t* name = nullptr;
	static void* instance = nullptr;

	//�Ăяo�����X���b�h�����L���Ă���E�B���h�E�ɑ��M���ꂽ���b�Z�[�W�ۗ̕�����Ă��镨���擾
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
		case WM_CREATE:
		{
			auto wnd = (LPWNDCLASSEX)GetWindowLongPtr(msg.hwnd, GWLP_HINSTANCE);
			name     = wnd->lpszClassName;
			instance = wnd->hInstance;
			break;
		}
		case WM_QUIT:
			UnregisterClass(name, HINSTANCE(instance));
			return false;
		default:
			break;
		}

		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
		TranslateMessage(&msg);
		//1�̃E�B�h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
		DispatchMessage(&msg);
	}

	return true;
}

// �R�}���h�L���[����
void CreateCommandQueue(ID3D12Device5* device, ID3D12CommandQueue** queue, const D3D12_COMMAND_LIST_TYPE& type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	auto hr = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&(*queue)));
	_ASSERT(hr == S_OK);
}

// �X���b�v�`�F�C������
void CreateSwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* queue, void* winHandle, IDXGISwapChain4** swap, const unsigned int& bufferCnt)
{
	RECT rect{};
	GetClientRect(HWND(winHandle), &rect);

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = bufferCnt;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = rect.bottom;
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = rect.right;

	auto hr = factory->CreateSwapChainForHwnd(queue, HWND(winHandle), &desc, nullptr, nullptr, (IDXGISwapChain1**)&(*swap));
	_ASSERT(hr == S_OK);
}

// �R�}���h�A���P�[�^����
void CreateCommandAllocator(ID3D12Device5* device, ID3D12CommandAllocator** allocator, const D3D12_COMMAND_LIST_TYPE& type)
{
	auto hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(&(*allocator)));
	_ASSERT(hr == S_OK);
}

// �R�}���h���X�g����
void CreateCommandList(ID3D12Device5* device, ID3D12GraphicsCommandList5** list, const D3D12_COMMAND_LIST_TYPE& type)
{
	auto hr = device->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&(*list)));
	_ASSERT(hr == S_OK);
}

// �q�[�v����
void CreateHeap(ID3D12Device5* device, ID3D12DescriptorHeap** heap, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const unsigned int& num = 1, 
	const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = flag;
	desc.NodeMask       = 0;
	desc.NumDescriptors = num;
	desc.Type           = type;

	auto hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&(*heap)));
	_ASSERT(hr == S_OK);
}

// �����_�[�^�[�Q�b�g�r���[����
void CreateRTV(ID3D12Device5* device, ID3D12DescriptorHeap* heap, ID3D12Resource* rsc, const unsigned int& index = 0)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format             = rsc->GetDesc().Format;
	desc.ViewDimension      = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += index * device->GetDescriptorHandleIncrementSize(heap->GetDesc().Type);
	device->CreateRenderTargetView(rsc, &desc, handle);
}

// �G���g���[�|�C���g
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	//�E�B���h�E����
	void* winHandle = nullptr;
	{
		WNDCLASSEX wnd{};
		wnd.cbSize        = sizeof(WNDCLASSEX);
		wnd.hbrBackground = CreateSolidBrush(COLOR_APPWORKSPACE);
		wnd.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		wnd.hIcon         = nullptr;
		wnd.hIconSm       = nullptr;
		wnd.hInstance     = GetModuleHandle(0);
		wnd.lpfnWndProc   = WNDPROC(WindowProc);
		wnd.lpszClassName = L"��������";
		wnd.lpszMenuName  = L"��������";
		wnd.style         = CS_HREDRAW | CS_VREDRAW;
		RegisterClassEx(&wnd);

		RECT rect{};
		rect.bottom = WINSIZE_Y;
		rect.right  = WINSIZE_X;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		winHandle = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, L"Raytracing", WS_OVERLAPPEDWINDOW, 0x80000000, 0x80000000,
			(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
		_ASSERT(winHandle != nullptr);

		ShowWindow(HWND(winHandle), SW_SHOW);
	}

#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
	Release(debug);
#endif

	//�t�@�N�g���[����
	IDXGIFactory7* factory = nullptr;
	{
		auto hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
		_ASSERT(hr == S_OK);
	}

	//�f�o�C�X����
	ID3D12Device5* device  = nullptr;
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1>adaptor = nullptr;
		unsigned int index    = 0;
		unsigned int revision = 0;
		for (unsigned int i = 0; factory->EnumAdapters1(i, &adaptor) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			for (const D3D_FEATURE_LEVEL& level : levels)
			{
				if (SUCCEEDED(D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device))))
				{
					goto Check;
				}
			}
		}

	Check:
		//���C�g���[�V���O�̃T�|�[�g�`�F�b�N
		D3D12_FEATURE_DATA_D3D12_OPTIONS5 option{};
		auto hr = device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS5, &option, sizeof(option));
		_ASSERT(hr == S_OK);

		//_ASSERT(option.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED);
	}

	//�R�}���h�L���[����
	ID3D12CommandQueue* queue = nullptr;
	{
		CreateCommandQueue(device, &queue, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	//�X���b�v�`�F�C������
	IDXGISwapChain4* swap = nullptr;
	{
		CreateSwapChain(factory, queue, winHandle, &swap, BACK_BUFFER);
	}

	//�R�}���h�A���P�[�^����
	std::vector<ID3D12CommandAllocator*>allocator;
	{
		DXGI_SWAP_CHAIN_DESC1 desc{};
		auto hr = swap->GetDesc1(&desc);
		_ASSERT(hr == S_OK);

		allocator.resize(desc.BufferCount);
		for (auto& i : allocator)
		{
			CreateCommandAllocator(device, &i, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		}
	}

	//�R�}���h���X�g����
	ID3D12GraphicsCommandList5* list = nullptr;
	{
		CreateCommandList(device, &list, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	//�t�F���X����
	Fence fence;
	{
		auto hr = device->CreateFence(fence.cnt, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence.ptr));
		_ASSERT(hr == S_OK);
	}

	//�����_�[�^�[�Q�b�g����
	Render render;
	{
		DXGI_SWAP_CHAIN_DESC1 desc{};
		auto hr = swap->GetDesc1(&desc);
		_ASSERT(hr == S_OK);
		CreateHeap(device, &render.heap, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc.BufferCount);

		render.rsc.resize(desc.BufferCount);
		for (unsigned int i = 0; i < desc.BufferCount; ++i)
		{
			hr = swap->GetBuffer(i, IID_PPV_ARGS(&render.rsc[i]));
			_ASSERT(hr == S_OK);

			CreateRTV(device, render.heap, render.rsc[i], i);
		}
	}

	while (CheckMsg() == true && !(GetKeyState(VK_ESCAPE) & 0x80))
	{

	}

	//�I��
	{
		for (auto& i : render.rsc)
		{
			Release(i);
		}
		Release(render.heap);
		Release(list);
		for (auto& i : allocator)
		{
			Release(i);
		}
		Release(swap);
		Release(queue);
		Release(device);
		Release(factory);
	}

	return 0;
}
