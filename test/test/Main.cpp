#include "Main.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// �E�B���h�E�R�[���o�b�N
__int64 __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam)
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

// �E�B���h�E����
void CreateWnd(const unsigned int& width, const unsigned int& height)
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
	rect.bottom = long(height);
	rect.right  = long(width);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	winHandle = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, L"Raytracing", WS_OVERLAPPEDWINDOW, 0x80000000, 0x80000000,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
	_ASSERT(winHandle != nullptr);

	ShowWindow(HWND(winHandle), SW_SHOW);
}

// �E�B���h�E�T�C�Y�̎擾
RECT GetWinSize(void)
{
	RECT rect{};
	GetClientRect(HWND(winHandle), &rect);

	return rect;
}

// �E�B���h�E���b�Z�[�W�̊m�F
bool CheckMsg(void)
{
	static MSG msg{};
	static const wchar_t* name = nullptr;
	static void* instance      = nullptr;

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

// D3D12�f�o�b�O�L��
void Debug(void)
{
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug>debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif
}

// �t�@�N�g���[�̐���
void CreateFactory(void)
{
	auto hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));
	_ASSERT(hr == S_OK);
}

// �f�o�C�X�̐���
void CreateDevice(void)
{
	Microsoft::WRL::ComPtr<IDXGIAdapter1>adaptor = nullptr;
	for (unsigned int i = 0; factory->EnumAdapters1(i, &adaptor) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 desc{};
		auto hr = adaptor->GetDesc1(&desc);
		_ASSERT(hr == S_OK);

		for (auto& level : levels)
		{
			if (SUCCEEDED(D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device))))
			{
				//���C�g���[�V���O�̃T�|�[�g�`�F�b�N
				D3D12_FEATURE_DATA_D3D12_OPTIONS5 option{};
				auto hr = device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS5, &option, sizeof(option));
				_ASSERT(hr == S_OK);

				if (option.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
				{
					goto End;
				}
			}
		}
	}

End:
	;
}

// �R�}���h�A���P�[�^�̐���
void CreateAllocator(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)
{
	auto hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	_ASSERT(hr == S_OK);
}

// �R�}���h�A���P�[�^�̃��Z�b�g
void Reset(const okmonn::Interface<ID3D12CommandAllocator>& allo)
{
	auto hr = allo->Reset();
	_ASSERT(hr == S_OK);
}

// �R�}���h���X�g�̐���
void CreateList(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)
{
	auto hr = device->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
	_ASSERT(hr == S_OK);
}

// �R�}���h���X�g�̃��Z�b�g
void Reset(const okmonn::Interface<ID3D12GraphicsCommandList5>& list, const okmonn::Interface<ID3D12CommandAllocator>& allo, ID3D12PipelineState* pipe = nullptr)
{
	auto hr = list->Reset(allo.get(), pipe);
	_ASSERT(hr == S_OK);
}

// �o���A
void Barrier(const okmonn::Interface<ID3D12GraphicsCommandList5>& list, okmonn::Interface<ID3D12Resource1>& rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after)
{
	D3D12_RESOURCE_BARRIER desc{};
	desc.Transition.pResource   = rsc.get();
	desc.Transition.StateAfter  = after;//�X�V��̏��
	desc.Transition.StateBefore = befor;//�X�V�O�̏��
	desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;//�J�ڃC���f�b�N�X�̐ݒ�
	desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;//���\�[�X�o���A�^�C�v�̐ݒ�

	list->ResourceBarrier(1, &desc);
}

// �R�}���h���X�g�̃N���[�Y
void Close(const okmonn::Interface<ID3D12GraphicsCommandList5>& list)
{
	auto hr = list->Close();
	_ASSERT(hr == S_OK);
}

// �R�}���h�L���[�̐���
void CreateQueue(const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;//GPU�^�C���A�E�g�ݒ�
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;//�D��x
	desc.Type     = type;//�R�}���h�^�C�v�̐ݒ�

	auto hr = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	_ASSERT(hr == S_OK);
}

// �R�}���h�̎��s
void Execution(const okmonn::Interface<ID3D12CommandQueue>& queue, ID3D12CommandList* const* lists, const size_t& listNum)
{
	queue->ExecuteCommandLists(unsigned int(listNum), lists);
}

// �t�F���X�̍X�V
void Signal(const okmonn::Interface<ID3D12CommandQueue>& queue, okmonn::Fence& fence)
{
	auto hr = queue->Signal(fence.get(), ++fence);
	_ASSERT(hr == S_OK);
}

// �t�F���X�̐���
void CreateFence(void)
{
	auto hr = device->CreateFence(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	_ASSERT(hr == S_OK);
}

// �X���b�v�`�F�C���̐���
void CreateSwap(const unsigned int& bufferCnt = 2)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;//�����x�̐ݒ�
	desc.BufferCount = bufferCnt;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//����I�v�V�����̐ݒ�
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;//���\�[�X�f�[�^�`���̐ݒ�
	desc.Height      = GetWinSize().bottom;
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;//�o�b�N�o�b�t�@�̃T�C�Y�ύX����̐ݒ�
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;//�o�b�N�o�b�t�@���e�̓���ݒ�
	desc.Width       = GetWinSize().right;

	auto hr = factory->CreateSwapChainForHwnd(queue.get(), HWND(winHandle), &desc, nullptr, nullptr, (IDXGISwapChain1**)&swap);
	_ASSERT(hr == S_OK);
}

// �o�b�N�o�b�t�@���̎擾
unsigned int GetBackBuffer(void)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	auto hr = swap->GetDesc1(&desc);
	_ASSERT(hr == S_OK);

	return desc.BufferCount;
}

// �q�[�v�̐���
void CreateHeap(okmonn::Interface<ID3D12DescriptorHeap>& heap, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const size_t& viewNum = 1, const bool& shader = false)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags = (shader == false)//�V�F�[�_�Q�Ƃ̐ݒ�
		? D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE
		: D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = unsigned int(viewNum);
	desc.Type           = type;//�q�[�v�^�C�v�̐ݒ�

	auto hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	_ASSERT(hr == S_OK);
}

// RTV�̐���
void RTV(const okmonn::Interface<ID3D12DescriptorHeap>& heap, const okmonn::Interface<ID3D12Resource1>& rsc, const size_t& index = 0)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format             = rsc->GetDesc().Format;//���\�[�X�f�[�^�`���̐ݒ�
	desc.Texture2D.MipSlice = 0;
	desc.ViewDimension      = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;//�\�����郊�\�[�X�^�C�v�̐ݒ�

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	device->CreateRenderTargetView(rsc.get(), &desc, handle);
}

// �����_�[�^�[�Q�b�g�̐���
void CreateRender(void)
{
	rtvRsc.resize(GetBackBuffer());

	CreateHeap(rtvHeap, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, rtvRsc.size());

	for (size_t i = 0; i < rtvRsc.size(); ++i)
	{
		auto hr = swap->GetBuffer(unsigned int(i), IID_PPV_ARGS(&rtvRsc[i]));
		_ASSERT(hr == S_OK);

		RTV(rtvHeap, rtvRsc[i], i);
	}
}

// �����_�[�^�[�Q�b�g�̃N���A
void ClearRtv(const okmonn::Interface<ID3D12GraphicsCommandList5>& list, const okmonn::Interface<ID3D12DescriptorHeap>& heap, const okmonn::Interface<ID3D12Resource1>& rsc, const float* color, const size_t& index)
{
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += device->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;

	list->ClearRenderTargetView(handle, color, 0, nullptr);
}

// �f�t�H���g�q�[�v�v���p�e�B�̎擾
D3D12_HEAP_PROPERTIES DefaultProp(void)
{
	D3D12_HEAP_PROPERTIES desc{};
	desc.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;//CPU�y�[�W�v���p�e�B�̐ݒ�
	desc.CreationNodeMask     = 0;
	desc.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;//�������v�[���̐ݒ�
	desc.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;//�q�[�v�^�C�v�̐ݒ�
	desc.VisibleNodeMask      = 0;

	return desc;
}

// �A�b�v���[�h�v���p�e�B�̎擾
D3D12_HEAP_PROPERTIES UploadProp(void)
{
	D3D12_HEAP_PROPERTIES desc{};
	desc.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;//CPU�y�[�W�v���p�e�B�̐ݒ�
	desc.CreationNodeMask     = 0;
	desc.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;//�������v�[���̐ݒ�
	desc.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;//�q�[�v�^�C�v�̐ݒ�
	desc.VisibleNodeMask      = 0;

	return desc;
}

// ���\�[�X�̐���
void CreateRsc(okmonn::Interface<ID3D12Resource1>& rsc, const D3D12_RESOURCE_FLAGS& flag, const size_t& size, const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_STATES& state)
{
	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = unsigned __int64(size);

	auto hr = device->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, state, nullptr, nullptr, IID_PPV_ARGS(&rsc));
	_ASSERT(hr == S_OK);
}

// �v���~�e�B�u�̐���
void CreatePrimitive(okmonn::Interface<ID3D12Resource1>& primitive, const std::initializer_list<okmonn::Vec3>& vertex)
{
	std::vector<okmonn::Vec3>tmp = vertex;
	CreateRsc(primitive, D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE, sizeof(tmp[0]) * tmp.size(), UploadProp(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

	void* buf = nullptr;
	auto hr = primitive->Map(0, nullptr, &buf);
	_ASSERT(hr == S_OK);
	std::memcpy(buf, tmp.data(), primitive->GetDesc().Width);
	primitive->Unmap(0, nullptr);
}

// �{�g�����x���̐���
void CreateBottom(okmonn::Acceleration& bottom, const okmonn::Interface<ID3D12Resource1>& primitive)
{
	D3D12_RAYTRACING_GEOMETRY_DESC geo{};
	geo.Flags                                = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	geo.Triangles.VertexBuffer.StartAddress  = primitive->GetGPUVirtualAddress();
	geo.Triangles.VertexBuffer.StrideInBytes = sizeof(okmonn::Vec3);
	geo.Triangles.VertexCount                = primitive->GetDesc().Width / geo.Triangles.VertexBuffer.StrideInBytes;
	geo.Triangles.VertexFormat               = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	geo.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input.NumDescs       = 1;
	input.pGeometryDescs = &geo;
	input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
}

// �G���g���[�|�C���g
int main()
{
	//tutorial 1
	CreateWnd(640, 480);

	//tutorial 2
	Debug();
	CreateFactory();
	CreateDevice();
	CreateAllocator();
	CreateList();
	CreateQueue();
	CreateFence();
	CreateSwap();
	CreateRender();
	//tutorial 3
	CreatePrimitive(triangle, { okmonn::Vec3(0.0f,     1.0f, 0.0f),
								okmonn::Vec3(0.866f,  -0.5f, 0.0f),
								okmonn::Vec3(-0.866f, -0.5f, 0.0f)});

	const float color[] =
	{
		0.4f, 0.6f, 0.2f, 1.0f
	};
	while (CheckMsg())
	{
		unsigned int index = swap->GetCurrentBackBufferIndex();

		Reset(allo);
		Reset(list, allo);

		Barrier(list, rtvRsc[index], D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		ClearRtv(list, rtvHeap, rtvRsc[index], color, index);
		Barrier(list, rtvRsc[index], D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

		Close(list);
		ID3D12CommandList* lists[] =
		{
			list.get()
		};
		Execution(queue, lists, _countof(lists));
		Signal(queue, fence);
		fence.Wait();
		swap->Present(0, 0);
	}

	return 0;
}