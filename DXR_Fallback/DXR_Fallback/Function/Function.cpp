#include "Function.h"
#include <wrl.h>
#include <cassert>

// �E�B���h�E�R�[���o�b�N
int64_t __stdcall WindowProc(void* hWnd, uint32_t message, uint64_t wParam, int64_t lParam)
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

void CreateWnd(void** handle, const std::uint32_t& sizeX, const std::uint32_t& sizeY)
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
	rect.bottom = sizeY;
	rect.right  = sizeX;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	(*handle) = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, L"��������", WS_OVERLAPPEDWINDOW, 0x80000000, 0x80000000,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
	assert((*handle) != nullptr);

	ShowWindow(HWND(*handle), SW_SHOW);
}

void CreateDevice(ID3D12Device5** dev, const D3D_FEATURE_LEVEL& level)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
	debug->Release();
#endif

	auto hr = D3D12CreateDevice(nullptr, level, IID_PPV_ARGS(&(*dev)));
	assert(hr == S_OK);
}

void CreateDevice(ID3D12RaytracingFallbackDevice** f_dev, ID3D12Device5* dev)
{
	auto hr = D3D12CreateRaytracingFallbackDevice(dev, CreateRaytracingFallbackDeviceFlags::None, 0, IID_PPV_ARGS(&(*f_dev)));
	assert(hr == S_OK);
}

void CreateAllocator(ID3D12CommandAllocator** allo, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type)
{
	auto hr = dev->CreateCommandAllocator(type, IID_PPV_ARGS(&(*allo)));
	assert(hr == S_OK);
}

void CreateQueue(ID3D12CommandQueue** queue, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	auto hr = dev->CreateCommandQueue(&desc, IID_PPV_ARGS(&(*queue)));
	assert(hr == S_OK);
}

void CreateList(ID3D12GraphicsCommandList4** list, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type)
{
	auto hr = dev->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&(*list)));
	assert(hr == S_OK);
}

void CreateList(ID3D12RaytracingFallbackCommandList** f_list, ID3D12RaytracingFallbackDevice* dev, ID3D12GraphicsCommandList4* list)
{
	dev->QueryRaytracingCommandList(list, IID_PPV_ARGS(&(*f_list)));
}

void CreateFence(ID3D12Fence1** fence, ID3D12Device5* dev, const std::uint64_t& initCnt)
{
	auto hr = dev->CreateFence(initCnt, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&(*fence)));
	assert(hr == S_OK);
}

void CreateSwapChain(IDXGISwapChain4** swap, void* win, ID3D12CommandQueue* queue, const std::uint32_t& backBuffNum)
{
	Microsoft::WRL::ComPtr<IDXGIFactory7>factory = nullptr;
	std::uint32_t flag = 0;
#ifdef _DEBUG
	flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
	auto hr = CreateDXGIFactory2(flag, IID_PPV_ARGS(&factory));
	assert(hr == S_OK);

	RECT rect{};
	GetClientRect(HWND(win), &rect);

	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = backBuffNum;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = rect.bottom;
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = rect.right;

	hr = factory->CreateSwapChainForHwnd(queue, HWND(win), &desc, nullptr, nullptr, (IDXGISwapChain1**)&(*swap));
	assert(hr == S_OK);
}

void CreateHeap(ID3D12DescriptorHeap** heap, ID3D12Device5* dev, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& rscNum, const bool& shader)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = (shader == false) ? D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = rscNum;
	desc.Type           = type;

	auto hr = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&(*heap)));
	assert(hr == S_OK);
}

void CreateRsc(ID3D12Resource1** rsc, IDXGISwapChain4* swap, const std::uint32_t& index)
{
	auto hr = swap->GetBuffer(index, IID_PPV_ARGS(&(*rsc)));
	assert(hr == S_OK);
}

void CreateRsc(ID3D12Resource1** rsc, ID3D12Device5* dev, const std::uint32_t& vertexStride, const std::uint32_t& vertexNum)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 0;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = vertexStride * vertexNum;

	auto hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, nullptr, IID_PPV_ARGS(&(*rsc)));
	assert(hr == S_OK);
}

void RTV(ID3D12Resource1* rsc, ID3D12Device5* dev, ID3D12DescriptorHeap* heap, const std::uint32_t& index)
{
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format             = rsc->GetDesc().Format;
	desc.Texture2D.MipSlice = 0;
	desc.ViewDimension      = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	dev->CreateRenderTargetView(rsc, &desc, handle);
}

void CreateAcceleration(ID3D12Resource1** scratch, ID3D12Resource1** result, ID3D12RaytracingFallbackDevice* dev, ID3D12RaytracingFallbackCommandList* list, ID3D12DescriptorHeap* heap, ID3D12Resource1* primitive, const std::uint32_t& vertexNum)
{
	D3D12_RAYTRACING_GEOMETRY_DESC geo{};
	geo.Flags = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	geo.Triangles.VertexBuffer.StartAddress  = primitive->GetGPUVirtualAddress();
	geo.Triangles.VertexCount                = vertexNum;
	geo.Triangles.VertexBuffer.StrideInBytes = primitive->GetDesc().Width / geo.Triangles.VertexCount;
	geo.Triangles.VertexFormat               = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	geo.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input.NumDescs       = 1;
	input.pGeometryDescs = &geo;
	input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	dev->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	prop.VisibleNodeMask      = 0;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = info.ScratchDataSizeInBytes;

	Microsoft::WRL::ComPtr<ID3D12Device5>device = nullptr;
	auto hr = dev->QueryInterface(IID_PPV_ARGS(&device));
	assert(hr == S_OK);

	hr = device->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, nullptr, IID_PPV_ARGS(&(*scratch)));
	assert(hr == S_OK);

	desc.Width = info.ResultDataMaxSizeInBytes;
	hr = device->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, dev->GetAccelerationStructureResourceState(), nullptr, nullptr, IID_PPV_ARGS(&(*result)));
	assert(hr == S_OK);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC build{};
	build.DestAccelerationStructureData    = (*result)->GetGPUVirtualAddress();
	build.Inputs                           = input;
	build.ScratchAccelerationStructureData = (*scratch)->GetGPUVirtualAddress();

	list->SetDescriptorHeaps(1, (ID3D12DescriptorHeap*const*)&heap);
	list->BuildRaytracingAccelerationStructure(&build, 0, nullptr);

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4>command = nullptr;
	hr = list->QueryInterface(IID_PPV_ARGS(&command));
	assert(hr == S_OK);

	Barrier((*result), command.Get());
	Close(command.Get());
}

void CreateAcceleration(ID3D12Resource1** scratch, ID3D12Resource1** result, ID3D12Resource1** instance, ID3D12RaytracingFallbackDevice* dev, ID3D12RaytracingFallbackCommandList* list, ID3D12DescriptorHeap* heap)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 0;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 0;

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
	desc.Width            = sizeof(D3D12_RAYTRACING_INSTANCE_DESC);

	Microsoft::WRL::ComPtr<ID3D12Device5>device = nullptr;
	auto hr = dev->QueryInterface(IID_PPV_ARGS(&device));
	assert(hr == S_OK);

	hr = device->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, 
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, nullptr, IID_PPV_ARGS(&(*instance)));
	assert(hr == S_OK);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
	input.DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	input.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	input.InstanceDescs = (*instance)->GetGPUVirtualAddress();
	input.NumDescs      = 1;
	input.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	dev->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);

	prop.Type  = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Width = info.ScratchDataSizeInBytes;
	hr = device->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, nullptr, IID_PPV_ARGS(&(*scratch)));
	assert(hr == S_OK);

	desc.Width = info.ResultDataMaxSizeInBytes;
	hr = device->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, dev->GetAccelerationStructureResourceState(), nullptr, nullptr, IID_PPV_ARGS(&(*result)));
	assert(hr == S_OK);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC build{};
	build.DestAccelerationStructureData    = (*result)->GetGPUVirtualAddress();
	build.Inputs                           = input;
	build.ScratchAccelerationStructureData = (*scratch)->GetGPUVirtualAddress();

	list->SetDescriptorHeaps(1, (ID3D12DescriptorHeap* const*)&heap);
	list->BuildRaytracingAccelerationStructure(&build, 0, nullptr);

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4>command = nullptr;
	hr = list->QueryInterface(IID_PPV_ARGS(&command));
	assert(hr == S_OK);

	Barrier((*result), command.Get());
	Close(command.Get());
}

void CreateGlobalRoot(ID3D12RootSignature** root, ID3D12RaytracingFallbackDevice* dev, std::vector<D3D12_STATE_SUBOBJECT>& subObj)
{
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
	desc.Desc_1_1 = {};
	desc.Version  = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

	Microsoft::WRL::ComPtr<ID3DBlob>sig   = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>error = nullptr;
	auto hr = dev->D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
	assert(hr == S_OK);

	hr = dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&(*root)));
	assert(hr == S_OK);

	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE , root });
}

void CreateLocalRoot(ID3D12RootSignature** root, ID3D12RaytracingFallbackDevice* dev, std::vector<D3D12_STATE_SUBOBJECT>& subObj)
{
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
	desc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	desc.Version        = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

	Microsoft::WRL::ComPtr<ID3DBlob>sig   = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>error = nullptr;
	auto hr = dev->D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
	assert(hr == S_OK);

	hr = dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&(*root)));
	assert(hr == S_OK);

	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE , root });
}

void CreateLocalRayGenRoot(ID3D12RootSignature** root, ID3D12RaytracingFallbackDevice* dev, std::vector<D3D12_STATE_SUBOBJECT>& subObj)
{
	D3D12_DESCRIPTOR_RANGE1 range[2];
	/*�o�̓f�[�^*/
	range[0].BaseShaderRegister                = 0;
	range[0].Flags                             = D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
	range[0].NumDescriptors                    = 1;
	range[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[0].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	range[0].RegisterSpace                     = 0;
	/*�����\��*/
	range[1].BaseShaderRegister                = 0;
	range[1].Flags                             = D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
	range[1].NumDescriptors                    = 1;
	range[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	range[1].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range[1].RegisterSpace                     = 0;

	D3D12_ROOT_PARAMETER1 param{};
	param.DescriptorTable.NumDescriptorRanges = std::uint32_t(_countof(range));
	param.DescriptorTable.pDescriptorRanges   = &range[0];
	param.ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param.ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
	desc.Desc_1_1.Flags         = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	desc.Desc_1_1.NumParameters = 1;
	desc.Desc_1_1.pParameters   = &param;
	desc.Version                = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>error = nullptr;
	auto hr = dev->D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
	assert(hr == S_OK);

	hr = dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&(*root)));
	assert(hr == S_OK);

	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE , root });
}

void CreatePipe(ID3D12RaytracingFallbackStateObject** pipe, ID3D12RaytracingFallbackDevice* dev, const std::vector<D3D12_STATE_SUBOBJECT>& subObj)
{
	D3D12_STATE_OBJECT_DESC desc{};
	desc.NumSubobjects = std::uint32_t(subObj.size());
	desc.pSubobjects   = subObj.data();
	desc.Type          = D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;

	auto hr = dev->CreateStateObject(&desc, IID_PPV_ARGS(&(*pipe)));
	assert(hr == S_OK);
}

bool CheckMsg(void)
{
	static MSG msg{};
	static const wchar_t* name = nullptr;
	static void* instance = nullptr;

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		switch (msg.message)
		{
		case WM_CREATE:
		{
			auto wnd = (LPWNDCLASSEX)GetWindowLongPtr(msg.hwnd, GWLP_HINSTANCE);
			name = wnd->lpszClassName;
			instance = wnd->hInstance;
			break;
		}
		case WM_QUIT:
			UnregisterClass(name, HINSTANCE(instance));
			return false;
		default:
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Reset(ID3D12CommandAllocator* allo)
{
	auto hr = allo->Reset();
	assert(hr == S_OK);
}

void Reset(ID3D12GraphicsCommandList4* list, ID3D12CommandAllocator* allo)
{
	auto hr = list->Reset(allo, nullptr);
	assert(hr == S_OK);
}

void Close(ID3D12GraphicsCommandList4* list)
{
	auto hr = list->Close();
	assert(hr == S_OK);
}

void Barrier(ID3D12Resource1* rsc, ID3D12GraphicsCommandList4* list)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags         = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = rsc;

	list->ResourceBarrier(1, &barrier);
}

void Barrier(ID3D12Resource1* rsc, ID3D12GraphicsCommandList4* list, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = rsc;
	barrier.Transition.StateAfter  = after;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	list->ResourceBarrier(1, &barrier);
}

std::uint32_t GetBackBuffNum(IDXGISwapChain4* swap)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	auto hr = swap->GetDesc1(&desc);
	assert(hr == S_OK);

	return desc.BufferCount;
}

std::uint32_t GetBuffIndex(IDXGISwapChain4* swap)
{
	return swap->GetCurrentBackBufferIndex();
}

void Copy(ID3D12Resource1* rsc, void* data)
{
	void* buf = nullptr;
	auto hr = rsc->Map(0, nullptr, &buf);
	assert(hr == S_OK);

	std::memcpy(buf, data, rsc->GetDesc().Width);

	rsc->Unmap(0, nullptr);
}

void Clear(ID3D12DescriptorHeap* heap, ID3D12Device5* dev, ID3D12GraphicsCommandList4* list, const std::uint32_t& index, const float* color, ID3D12DescriptorHeap* depth)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = heap->GetCPUDescriptorHandleForHeapStart();
	rtv.ptr += dev->GetDescriptorHandleIncrementSize(heap->GetDesc().Type) * index;
	D3D12_CPU_DESCRIPTOR_HANDLE* dsv = (depth == nullptr) ? nullptr : &depth->GetCPUDescriptorHandleForHeapStart();

	list->OMSetRenderTargets(1, &rtv, false, dsv);
	list->ClearRenderTargetView(rtv, color, 0, nullptr);
}

void AccelerationUpDate(ID3D12Resource1* rsc, ID3D12Resource1* bottom)
{
	D3D12_RAYTRACING_INSTANCE_DESC* buf = nullptr;
	auto hr = rsc->Map(0, nullptr, (void**)&buf);
	assert(hr == S_OK);

	buf->AccelerationStructure = bottom->GetGPUVirtualAddress();
	buf->Flags = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	buf->InstanceContributionToHitGroupIndex = 0;
	buf->InstanceID = 0;
	buf->InstanceMask = 0xff;
	std::memset(buf->Transform, 0, sizeof(buf->Transform));
	for (std::uint32_t i = 0; i < 3; ++i) {
		buf->Transform[i][i] = 1.0f;
	}

	rsc->Unmap(0, nullptr);
}

void ShaderCompile(const std::wstring& filePath, const std::wstring& model, IDxcBlob** blob, D3D12_DXIL_LIBRARY_DESC& dxil, std::vector<D3D12_EXPORT_DESC>& exportDesc, 
	std::vector<D3D12_STATE_SUBOBJECT>& subObj, const wchar_t* exportName[], const std::uint32_t& exportNameNum)
{
	Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
	auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcIncludeHandler>handler = nullptr;
	library->CreateIncludeHandler(&handler);
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
	library->CreateBlobFromFile(filePath.c_str(), nullptr, &encode);
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcCompiler>compiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	assert(hr == S_OK);

	Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
	hr = compiler->Compile(encode.Get(), filePath.c_str(), L"", model.c_str(), nullptr, 0, nullptr, 0, handler.Get(), &result);
	assert(hr == S_OK);

	result->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		hr = result->GetResult(&(*blob));
		assert(hr == S_OK);
	}
	else
	{
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print = nullptr;
		Microsoft::WRL::ComPtr<IDxcBlobEncoding>print16 = nullptr;
		hr = result->GetErrorBuffer(&print);
		assert(hr == S_OK);

		hr = library->GetBlobAsUtf16(print.Get(), &print16);
		assert(hr == S_OK);
		wprintf(L"%*s", (int)print16->GetBufferSize() / 2, (LPCWSTR)print16->GetBufferPointer());
	}

	exportDesc.resize(exportNameNum);
	for (std::uint32_t i = 0; i < exportDesc.size(); ++i) {
		exportDesc[i].ExportToRename = nullptr;
		exportDesc[i].Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
		exportDesc[i].Name           = exportName[i];
	}

	dxil.DXILLibrary.BytecodeLength  = (*blob)->GetBufferSize();
	dxil.DXILLibrary.pShaderBytecode = (*blob)->GetBufferPointer();
	dxil.NumExports                  = std::uint32_t(exportDesc.size());
	dxil.pExports                    = exportDesc.data();

	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, &dxil });
}

void SetHitGroup(D3D12_HIT_GROUP_DESC& hitDesc, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const wchar_t* closesthit, const wchar_t* hitName, const wchar_t* anyhit, const wchar_t* intersection)
{
	hitDesc.AnyHitShaderImport       = anyhit;
	hitDesc.ClosestHitShaderImport   = closesthit;
	hitDesc.HitGroupExport           = hitName;
	hitDesc.IntersectionShaderImport = intersection;
	hitDesc.Type                     = D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES;
	
	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP, &hitDesc });
}

void SetShaderConfig(D3D12_RAYTRACING_SHADER_CONFIG& config, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const std::uint32_t& payloadSize, const std::uint32_t& attributeSize)
{
	config.MaxAttributeSizeInBytes = attributeSize;
	config.MaxPayloadSizeInBytes   = payloadSize;

	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, &config });
}

void ExportAssociation(D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION& association, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const wchar_t* exportName[], const std::uint32_t& exportNameNum)
{
	association.NumExports            = exportNameNum;
	association.pExports              = exportName;
	association.pSubobjectToAssociate = &(*subObj.rbegin());

	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION, &association });
}

void SetPipeConfig(D3D12_RAYTRACING_PIPELINE_CONFIG& config, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const std::uint32_t& depth)
{
	config.MaxTraceRecursionDepth = depth;

	subObj.push_back({ D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG, &config });
}

void Execution(ID3D12CommandQueue* queue, const std::initializer_list<ID3D12GraphicsCommandList4*>& lists)
{
	std::vector<ID3D12CommandList*>list;
	for (auto& i : lists) {
		list.push_back(i);
	}

	queue->ExecuteCommandLists(std::uint32_t(list.size()), list.data());
}

void Wait(ID3D12Fence1* fence, ID3D12CommandQueue* queue, IDXGISwapChain4* swap, std::uint64_t& fenceCnt)
{
	auto hr = queue->Signal(fence, ++fenceCnt);
	assert(hr == S_OK);

	if (fence->GetCompletedValue() != fenceCnt) {
		void* handle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
		hr = fence->SetEventOnCompletion(fenceCnt, handle);
		assert(hr == S_OK);

		WaitForSingleObject(handle, INFINITE);
		CloseHandle(handle);
	}

	hr = swap->Present(1, 0);
	assert(hr == S_OK);
}