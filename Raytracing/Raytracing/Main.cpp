#include "ShaderLibrary.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

// クリアカラー
float color[] = {
	1.0f, 1.0f, 1.0f, 1.0f
};

// ウィンドウコールバック
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

// メッセージの確認
bool CheckMsg(void)
{
	static MSG msg{};
	static const wchar_t* name = nullptr;
	static void* instance = nullptr;

	//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
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

		//仮想キーメッセージを文字メッセージに変換
		TranslateMessage(&msg);
		//1つのウィドウプロシージャにメッセージを送出する
		DispatchMessage(&msg);
	}

	return true;
}

// コマンドキュー生成
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

// スワップチェイン生成
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

// コマンドアロケータ生成
void CreateCommandAllocator(ID3D12Device5* device, ID3D12CommandAllocator** allocator, const D3D12_COMMAND_LIST_TYPE& type)
{
	auto hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(&(*allocator)));
	_ASSERT(hr == S_OK);
}

// コマンドリスト生成
void CreateCommandList(ID3D12Device5* device, ID3D12GraphicsCommandList5** list, const D3D12_COMMAND_LIST_TYPE& type)
{
	auto hr = device->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&(*list)));
	_ASSERT(hr == S_OK);
}

// ビューポートのセット
void Viewport(ID3D12GraphicsCommandList5* list, const unsigned int& sizeX, const unsigned int& sizeY)
{
	D3D12_VIEWPORT view{};
	view.Height   = float(sizeY);
	view.MaxDepth = 1.0f;
	view.Width    = float(sizeX);

	list->RSSetViewports(1, &view);
}

// シザーのセット
void Scissor(ID3D12GraphicsCommandList5* list, const unsigned int& sizeX, const unsigned int& sizeY)
{
	RECT scissor{};
	scissor.bottom = sizeY;
	scissor.right  = sizeX;

	list->RSSetScissorRects(1, &scissor);
}

// リソースバリア
void Barrier(ID3D12GraphicsCommandList5* list, ID3D12Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Transition.pResource   = rsc;
	barrier.Transition.StateAfter  = after;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	list->ResourceBarrier(1, &barrier);
}

// UAVバリア
void Barrier(ID3D12GraphicsCommandList5* list, ID3D12Resource* rsc)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = rsc;

	list->ResourceBarrier(1, &barrier);
}

// ヒープ生成
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

// リソース生成
void CreateRsc(ID3D12Device5* device, ID3D12Resource** rsc, const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc,
	const D3D12_RESOURCE_STATES& state, const D3D12_HEAP_FLAGS& flag = D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, const D3D12_CLEAR_VALUE* clear = nullptr)
{
	auto hr = device->CreateCommittedResource(&prop, flag, &desc, state, clear, IID_PPV_ARGS(&(*rsc)));
	_ASSERT(hr == S_OK);
}

// レンダーターゲットビュー生成
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

// レンダーターゲットのクリア
void ClearRenderTarget(ID3D12Device5* device, ID3D12GraphicsCommandList5* list, const Render& render, const unsigned int& index, float* color, D3D12_CPU_DESCRIPTOR_HANDLE* depth = nullptr)
{
	auto rtv = render.heap->GetCPUDescriptorHandleForHeapStart();
	rtv.ptr += index * device->GetDescriptorHandleIncrementSize(render.heap->GetDesc().Type);

	list->OMSetRenderTargets(1, &rtv, false, depth);

	list->ClearRenderTargetView(rtv, color, 0, nullptr);
}

// トップレベル加速構造の生成
void CreateTopLevel(ID3D12Device5* device, Acceleration& acceleration, const Acceleration& bottom)
{
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

		CreateRsc(device, &acceleration.instance, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	}

	acceleration.input.DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	acceleration.input.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	acceleration.input.NumDescs      = 1;
	acceleration.input.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	acceleration.input.InstanceDescs = acceleration.instance->GetGPUVirtualAddress();

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	device->GetRaytracingAccelerationStructurePrebuildInfo(&acceleration.input, &info);

	{
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

		CreateRsc(device, &acceleration.scratch, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

		desc.Width = info.ResultDataMaxSizeInBytes;
		CreateRsc(device, &acceleration.result, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);
	}

	D3D12_RAYTRACING_INSTANCE_DESC* ptr = nullptr;
	auto hr = acceleration.instance->Map(0, nullptr, (void**)&ptr);
	_ASSERT(hr == S_OK);
	ptr->AccelerationStructure               = bottom.result->GetGPUVirtualAddress();
	ptr->Flags                               = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	ptr->InstanceContributionToHitGroupIndex = 0;
	ptr->InstanceID                          = 0;
	ptr->InstanceMask                        = 1;
	float mat[3][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f }
	};
	memcpy(ptr->Transform, &mat, sizeof(mat));
	acceleration.instance->Unmap(0, nullptr);
}

// ボトムレベル加速構造の生成
void CreateBottomLevel(ID3D12Device5* device, Acceleration& acceleration, ID3D12Resource* vertex)
{
	acceleration.geoDesc.Flags                                = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	acceleration.geoDesc.Triangles.VertexBuffer.StartAddress  = vertex->GetGPUVirtualAddress();
	acceleration.geoDesc.Triangles.VertexBuffer.StrideInBytes = sizeof(Vector3);
	acceleration.geoDesc.Triangles.VertexCount                = unsigned int(vertex->GetDesc().Width / acceleration.geoDesc.Triangles.VertexBuffer.StrideInBytes);
	acceleration.geoDesc.Triangles.VertexFormat               = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	acceleration.geoDesc.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

	acceleration.input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
	acceleration.input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
	acceleration.input.NumDescs       = 1;
	acceleration.input.pGeometryDescs = &acceleration.geoDesc;
	acceleration.input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
	device->GetRaytracingAccelerationStructurePrebuildInfo(&acceleration.input, &info);

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

	CreateRsc(device, &acceleration.scratch, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	desc.Width = info.ResultDataMaxSizeInBytes;
	CreateRsc(device, &acceleration.result, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);
}

// 加速構造のビルド
void BuildAcceleration(ID3D12GraphicsCommandList5* list, const Acceleration& acceleration)
{
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
	desc.DestAccelerationStructureData    = acceleration.result->GetGPUVirtualAddress();
	desc.Inputs                           = acceleration.input;
	desc.ScratchAccelerationStructureData = acceleration.scratch->GetGPUVirtualAddress();
	if (acceleration.instance != nullptr)
	{
		//desc.SourceAccelerationStructureData = acceleration.instance->GetGPUVirtualAddress();
	}

	list->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);

	Barrier(list, acceleration.result);
}

// レイジェネレーションルートシグネチャ情報取得
RootDesc GetRayGenDesc()
{
	RootDesc root(2, 1);
	//output
	root.range[0].BaseShaderRegister                = 0;
	root.range[0].NumDescriptors                    = 1;
	root.range[0].OffsetInDescriptorsFromTableStart = 0;
	root.range[0].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	root.range[0].RegisterSpace                     = 0;

	//scene
	root.range[1].BaseShaderRegister                = 0;
	root.range[1].NumDescriptors                    = 1;
	root.range[1].OffsetInDescriptorsFromTableStart = 1;
	root.range[1].RangeType                         = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	root.range[1].RegisterSpace                     = 0;
	
	root.param[0].DescriptorTable.NumDescriptorRanges = root.range.size();
	root.param[0].DescriptorTable.pDescriptorRanges   = root.range.data();
	root.param[0].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

	root.desc.Flags         = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
	root.desc.NumParameters = root.param.size();
	root.desc.pParameters   = root.param.data();

	return root;
}

// ルートシグネチャ生成
void CreateRoot(ID3D12Device5* device, ID3D12RootSignature** root, const D3D12_ROOT_SIGNATURE_DESC& desc)
{
	Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
	auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1, &sig, nullptr);
	_ASSERT(hr == S_OK);

	hr = device->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&(*root)));
	_ASSERT(hr == S_OK);
}

// ローカルルートシグネチャ生成
void CreateLocalRoot(ID3D12Device5* device, Root& local, const D3D12_ROOT_SIGNATURE_DESC& desc)
{
	CreateRoot(device, &local.root, desc);

	local.sub.pDesc = &local.root;
	local.sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
}

// グローバルルートシグネチャ生成
void CreateGlobalRoot(ID3D12Device5* device, Root& global, const D3D12_ROOT_SIGNATURE_DESC& desc)
{
	CreateRoot(device, &global.root, desc);

	global.sub.pDesc = &global.root;
	global.sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
}

// コマンド初期化
void InitCommand(ID3D12CommandAllocator* allocator, ID3D12GraphicsCommandList5* list, ID3D12PipelineState* pipe = nullptr)
{
	auto hr = allocator->Reset();
	_ASSERT(hr == S_OK);

	hr = list->Reset(allocator, pipe);
	_ASSERT(hr == S_OK);
}

// コマンド実行
void Execution(ID3D12CommandQueue* queue, IDXGISwapChain4* swap, ID3D12GraphicsCommandList5* list, Fence& fence)
{
	list->Close();
	queue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&list);

	auto hr = queue->Signal(fence.ptr, ++fence.cnt);
	_ASSERT(hr == S_OK);

	hr = swap->Present(0, 0);
	_ASSERT(hr == S_OK);

	if (fence.ptr->GetCompletedValue() != fence.cnt)
	{
		hr = fence.ptr->SetEventOnCompletion(fence.cnt, fence.event);
		_ASSERT(hr == S_OK);

		WaitForSingleObject(fence.event, INFINITE);
	}
}

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT(hr == S_OK);

	//ウィンドウ生成
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
		wnd.lpszClassName = L"おかもん";
		wnd.lpszMenuName  = L"おかもん";
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

	//ファクトリー生成
	IDXGIFactory7* factory = nullptr;
	{
		auto hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
		_ASSERT(hr == S_OK);
	}

	//デバイス生成
	ID3D12Device5* device  = nullptr;
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter1>adaptor = nullptr;
		unsigned int index    = 0;
		unsigned int revision = 0;
		for (unsigned int i = 0; factory->EnumAdapters1(i, &adaptor) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 desc{};
			auto hr = adaptor->GetDesc1(&desc);
			_ASSERT(hr == S_OK);

			for (const D3D_FEATURE_LEVEL& level : levels)
			{
				if (SUCCEEDED(D3D12CreateDevice(adaptor.Get(), level, IID_PPV_ARGS(&device))))
				{
					goto Check;
				}
			}
		}

	Check:
		//レイトレーシングのサポートチェック
		D3D12_FEATURE_DATA_D3D12_OPTIONS5 option{};
		auto hr = device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_D3D12_OPTIONS5, &option, sizeof(option));
		_ASSERT(hr == S_OK);

		_ASSERT(option.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED);
	}

	//コマンドキュー生成
	ID3D12CommandQueue* queue = nullptr;
	{
		CreateCommandQueue(device, &queue, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	//スワップチェイン生成
	IDXGISwapChain4* swap = nullptr;
	{
		CreateSwapChain(factory, queue, winHandle, &swap, BACK_BUFFER);
	}

	//コマンドアロケータ生成
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

	//コマンドリスト生成
	ID3D12GraphicsCommandList5* list = nullptr;
	{
		CreateCommandList(device, &list, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	}

	//フェンス生成
	Fence fence;
	{
		auto hr = device->CreateFence(fence.cnt, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence.ptr));
		_ASSERT(hr == S_OK);
	}

	//レンダーターゲット生成
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

	//図形
	ID3D12Resource* box = nullptr;
	{
		const Vector3 vertex[] = {
			Vector3(-1.0f,  1.0f, 0.0f),
			Vector3( 1.0f,  1.0f, 0.0f),
			Vector3(-1.0f, -1.0f, 0.0f),
			//Vector3( 1.0f, -1.0f, 0.0f)
		};

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
		desc.Width            = sizeof(Vector3) * _countof(vertex);

		CreateRsc(device, &box, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

		void* ptr = nullptr;
		auto hr = box->Map(0, nullptr, &ptr);
		_ASSERT(hr == S_OK);
		memcpy(ptr, vertex, sizeof(vertex));
		box->Unmap(0, nullptr);
	}

	//ボトムレベル加速構造
	Acceleration bottom;
	{
		CreateBottomLevel(device, bottom, box);
		InitCommand(allocator[0], list);
		BuildAcceleration(list, bottom);
		Execution(queue, swap, list, fence);
	}

	//トップレベル加速構造
	Acceleration top;
	{
		CreateTopLevel(device, top, bottom);
		InitCommand(allocator[0], list);
		BuildAcceleration(list, top);
		Execution(queue, swap, list, fence);
	}

	ShaderLibrary::Get().Compile(L"sample.hlsl", L"", { kRayGenShader, kMissShader, kClosestHitShader }, L"lib_6_3");

	std::array<D3D12_STATE_SUBOBJECT, 10>sub;
	unsigned int index = 0;
	Root global;
	CreateGlobalRoot(device, global, {});
	ID3D12StateObject* pipe = nullptr;
	{
		sub[index++] = global.sub;

		sub[index++] = ShaderLibrary::Get().GetInfo(L"sample.hlsl").sub;

		Hit hit(nullptr, kClosestHitShader, kHitGroup);
		sub[index++] = hit.sub;

		Root rayGen;
		CreateLocalRoot(device, rayGen, GetRayGenDesc().desc);
		sub[index] = rayGen.sub;
		unsigned int rootIndex = index++;
		Association rayGenAsso(&kRayGenShader, 1, &sub[rootIndex]);
		sub[index++] = rayGenAsso.sub;

		D3D12_ROOT_SIGNATURE_DESC empty{};
		empty.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
		Root miss;
		CreateLocalRoot(device, miss, empty);
		sub[index] = miss.sub;
		rootIndex = index++;
		const wchar_t* missName[] = {
			kMissShader,
			kClosestHitShader
		};
		Association missAsso(missName, _countof(missName), &sub[rootIndex]);
		sub[index++] = missAsso.sub;

		ShaderConfig sConfig(sizeof(float) * 2, sizeof(float) * 1);
		sub[index] = sConfig.sub;
		unsigned int sIndex = index++;
		const wchar_t* shaderExpo[] = {
			kMissShader,
			kClosestHitShader,
			kRayGenShader
		};
		Association sAsso(shaderExpo, _countof(shaderExpo), &sub[sIndex]);
		sub[index++] = sAsso.sub;

		PipeConfig pConfig(0);
		sub[index++] = pConfig.sub;

		D3D12_STATE_OBJECT_DESC desc{};
		desc.NumSubobjects = sub.size();
		desc.pSubobjects   = sub.data();
		desc.Type          = D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
		auto hr = device->CreateStateObject(&desc, IID_PPV_ARGS(&pipe));
		_ASSERT(hr == S_OK);
	}

	ID3D12Resource* shaderTbl  = nullptr;
	unsigned int shaderTblSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	{
		shaderTblSize += 8;
		shaderTblSize  = ((shaderTblSize + (D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT - 1)) / D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT) * D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT;
	
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
		desc.Width            = shaderTblSize * 3;

		CreateRsc(device, &shaderTbl, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

		unsigned int* data = nullptr;
		hr = shaderTbl->Map(0, nullptr, (void**)&data);
		_ASSERT(hr == S_OK);

		Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>rootProp = nullptr;
		hr = pipe->QueryInterface(IID_PPV_ARGS(&rootProp));
		_ASSERT(hr == S_OK);

		//レイジェネレーション用
		memcpy(data, rootProp->GetShaderIdentifier(kRayGenShader), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		//ミスヒット用
		memcpy(data + shaderTblSize, rootProp->GetShaderIdentifier(kMissShader), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		//ヒット用
		memcpy(data + (shaderTblSize * 2), rootProp->GetShaderIdentifier(kHitGroup), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

		shaderTbl->Unmap(0, nullptr);
	}

	ID3D12DescriptorHeap* outputHeap = nullptr;
	ID3D12Resource* outputRsc = nullptr;
	{
		DXGI_SWAP_CHAIN_DESC1 swapDesc{};
		hr = swap->GetDesc1(&swapDesc);
		_ASSERT(hr == S_OK);

		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.CreationNodeMask     = 0;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
		prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		prop.VisibleNodeMask      = 0;

		D3D12_RESOURCE_DESC desc{};
		desc.DepthOrArraySize = 1;
		desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Height           = swapDesc.Height;
		desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.MipLevels        = 1;
		desc.SampleDesc       = { 1, 0 };
		desc.Width            = swapDesc.Width;
		
		CreateRsc(device, &outputRsc, prop, desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);

		CreateHeap(device, &outputHeap, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	
		D3D12_UNORDERED_ACCESS_VIEW_DESC uav{};
		uav.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
		device->CreateUnorderedAccessView(outputRsc, nullptr, &uav, outputHeap->GetCPUDescriptorHandleForHeapStart());

		D3D12_SHADER_RESOURCE_VIEW_DESC srv{};
		srv.RaytracingAccelerationStructure.Location = top.result->GetGPUVirtualAddress();
		srv.Shader4ComponentMapping                  = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srv.ViewDimension                            = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
		auto handle = outputHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += device->GetDescriptorHandleIncrementSize(outputHeap->GetDesc().Type);
		device->CreateShaderResourceView(nullptr, &srv, handle);
	}

	while (CheckMsg() == true)
	{
		unsigned int index = swap->GetCurrentBackBufferIndex();
		InitCommand(allocator[index], list);

		Barrier(list, render.rsc[index], D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		ClearRenderTarget(device, list, render, index, color);
		Barrier(list, render.rsc[index], D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

		Execution(queue, swap, list, fence);
	}

	//終了
	{
		Release(outputRsc);
		Release(outputHeap);
		Release(shaderTbl);
		Release(pipe);
		Release(global.root);
		Release(top.instance);
		Release(top.result);
		Release(top.scratch);
		Release(bottom.instance);
		Release(bottom.result);
		Release(bottom.scratch);
		Release(box);
		for (auto& i : render.rsc)
		{
			Release(i);
		}
		Release(render.heap);
		Release(fence.ptr);
		CloseHandle(fence.event);
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

	CoUninitialize();
	return 0;
}
