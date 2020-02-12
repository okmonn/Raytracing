#include <vector>
#include <cstdint>
#include <cassert>

#include <wrl.h>
#include <d3d12.h>
#include <dxcapi.h>
#include <dxgi1_6.h>
#include <D3D12RaytracingFallback.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "FallbackLayer.lib")
#pragma comment(lib, "WinPixEventRuntime.lib")
#pragma comment(lib, "dxcompiler.lib")

/*ウィンドウコールバック*/
std::int64_t __stdcall WindowProc(void* hWnd, std::uint32_t message, std::uint64_t wParam, std::int64_t lParam) {
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

/*ウィンドウメッセージの確認*/
bool CheckMsg(void) {
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

int main() {
#ifdef _DEBUG
	ID3D12Debug3* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	/*ウィンドウの生成*/
	void* win = nullptr;
	const std::uint32_t sizeX = 640;
	const std::uint32_t sizeY = 480;
	{
		WNDCLASSEX wnd{};
		wnd.cbSize        = sizeof(wnd);
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
		rect.bottom = sizeY;
		rect.right  = sizeX;
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		win = CreateWindowEx(WS_EX_ACCEPTFILES, wnd.lpszClassName, L"おかもん", WS_OVERLAPPEDWINDOW, 0x80000000, 0x80000000,
			(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
		assert(win != nullptr);

		ShowWindow(HWND(win), SW_SHOW);
	}
	/*デバイスの生成*/
	ID3D12Device5* dev = nullptr;
	ID3D12RaytracingFallbackDevice* f_dev = nullptr;
	{
		auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&dev));
		assert(hr == S_OK);

		hr = D3D12CreateRaytracingFallbackDevice(dev, CreateRaytracingFallbackDeviceFlags::None, 0, IID_PPV_ARGS(&f_dev));
		assert(hr == S_OK);
	}
	/*コマンドキューの生成*/
	ID3D12CommandQueue* queue = nullptr;
	{
		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Type     = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;

		auto hr = dev->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
		assert(hr == S_OK);
	}
	/*コマンドアロケータの生成*/
	ID3D12CommandAllocator* allo = nullptr;
	{
		auto hr = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allo));
		assert(hr == S_OK);
	}
	/*コマンドリストの生成*/
	ID3D12GraphicsCommandList4* list = nullptr;
	ID3D12RaytracingFallbackCommandList* f_list = nullptr;
	{
		auto hr = dev->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAGS::D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&list));
		assert(hr == S_OK);

		f_dev->QueryRaytracingCommandList(list, IID_PPV_ARGS(&f_list));
	}
	/*フェンスの生成*/
	ID3D12Fence1* fence = nullptr;
	std::uint64_t fenceCnt = 0;
	{
		auto hr = dev->CreateFence(fenceCnt, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		assert(hr == S_OK);
	}
	/*スワップチェインの生成*/
	IDXGISwapChain4* swap = nullptr;
	const std::uint32_t buffCnt = 2;
	{
		Microsoft::WRL::ComPtr<IDXGIFactory7>factory = nullptr;
		std::uint32_t flag = 0;
#ifdef _DEBUG
		flag = DXGI_CREATE_FACTORY_DEBUG;
#endif
		auto hr = CreateDXGIFactory2(flag, IID_PPV_ARGS(&factory));
		assert(hr == S_OK);

		DXGI_SWAP_CHAIN_DESC1 desc{};
		desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.BufferCount = buffCnt;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.Flags       = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Height      = sizeY;
		desc.SampleDesc  = { 1, 0 };
		desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
		desc.Stereo      = false;
		desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Width       = sizeX;

		hr = factory->CreateSwapChainForHwnd(queue, HWND(win), &desc, nullptr, nullptr, (IDXGISwapChain1**)&swap);
		assert(hr == S_OK);
	}
	/*レンダーターゲットの生成*/
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	std::vector<ID3D12Resource1*>rtvRsc(buffCnt);
	{
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask       = 0;
			desc.NumDescriptors = std::uint32_t(rtvRsc.size());
			desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

			auto hr = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&rtvHeap));
			assert(hr == S_OK);
		}
		{
			for (std::uint32_t i = 0; i < rtvRsc.size(); ++i) {
				auto hr = swap->GetBuffer(i, IID_PPV_ARGS(&rtvRsc[i]));
				assert(hr == S_OK);

				D3D12_RENDER_TARGET_VIEW_DESC desc{};
				desc.Format        = rtvRsc[i]->GetDesc().Format;
				desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;

				auto handle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
				handle.ptr += dev->GetDescriptorHandleIncrementSize(rtvHeap->GetDesc().Type) * i;
				dev->CreateRenderTargetView(rtvRsc[i], &desc, handle);
			}
		}
	}
	/*サブオブジェクト関連*/
	std::vector<D3D12_STATE_SUBOBJECT>subObj;
	subObj.reserve(12);
	/*シェーダのコンパイル*/
	const wchar_t* fileName = L"HLSL/Test.hlsl";
	const wchar_t* model = L"lib_6_3";
	const wchar_t* exportName[] = {
		L"RayGen", L"Miss", L"ClosestHit"
	};
	IDxcBlob* blob = nullptr;
	D3D12_DXIL_LIBRARY_DESC dxil{};
	D3D12_STATE_SUBOBJECT dxilSub{};
	std::vector<D3D12_EXPORT_DESC>exportDesc(_countof(exportName));
	{
		{
			Microsoft::WRL::ComPtr<IDxcLibrary>library = nullptr;
			auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
			assert(hr == S_OK);

			Microsoft::WRL::ComPtr<IDxcIncludeHandler>handler = nullptr;
			library->CreateIncludeHandler(&handler);
			assert(hr == S_OK);

			Microsoft::WRL::ComPtr<IDxcBlobEncoding>encode = nullptr;
			library->CreateBlobFromFile(fileName, nullptr, &encode);
			assert(hr == S_OK);

			Microsoft::WRL::ComPtr<IDxcCompiler>compiler = nullptr;
			hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
			assert(hr == S_OK);

			Microsoft::WRL::ComPtr<IDxcOperationResult>result = nullptr;
			hr = compiler->Compile(encode.Get(), fileName, L"", model, nullptr, 0, nullptr, 0, handler.Get(), &result);
			assert(hr == S_OK);

			result->GetStatus(&hr);
			if (SUCCEEDED(hr))
			{
				hr = result->GetResult(&blob);
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
		}
		for (std::uint32_t i = 0; i < exportDesc.size(); ++i) {
			exportDesc[i].ExportToRename = nullptr;
			exportDesc[i].Flags          = D3D12_EXPORT_FLAGS::D3D12_EXPORT_FLAG_NONE;
			exportDesc[i].Name           = exportName[i];
		}

		dxil.DXILLibrary.BytecodeLength  = blob->GetBufferSize();
		dxil.DXILLibrary.pShaderBytecode = blob->GetBufferPointer();
		dxil.NumExports                  = std::uint32_t(exportDesc.size());
		dxil.pExports                    = exportDesc.data();

		dxilSub.pDesc = &dxil;
		dxilSub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
		subObj.push_back(dxilSub);
	}
	/*ヒットグループの設定*/
	const wchar_t* hitName = L"hit";
	const wchar_t* closestName = exportName[2];
	D3D12_HIT_GROUP_DESC hit{};
	D3D12_STATE_SUBOBJECT hitSub{};
	{
		hit.AnyHitShaderImport = nullptr;
		hit.ClosestHitShaderImport   = closestName;
		hit.HitGroupExport           = hitName;
		hit.IntersectionShaderImport = nullptr;
		hit.Type                     = D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES;
	
		hitSub.pDesc = &hit;
		hitSub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
		subObj.push_back(hitSub);
	}
	/*グローバルルートシグネチャの生成*/
	ID3D12RootSignature* global = nullptr;
	D3D12_STATE_SUBOBJECT globalSub{};
	{
		D3D12_DESCRIPTOR_RANGE1 range[] = {
			{ D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_NONE, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
			{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_NONE, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
		};
		D3D12_ROOT_PARAMETER1 param{};
		param.DescriptorTable.NumDescriptorRanges = _countof(range);
		param.DescriptorTable.pDescriptorRanges   = range;
		param.ParameterType                       = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param.ShaderVisibility                    = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
		D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
		desc.Desc_1_1.Flags         = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_NONE;
		desc.Desc_1_1.NumParameters = 1;
		desc.Desc_1_1.pParameters   = &param;
		desc.Version                = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

		Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob>error = nullptr;
		if (f_dev->UsingRaytracingDriver()) {
			auto hr = D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
			assert(hr == S_OK);

			hr = dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&global));
			assert(hr == S_OK);
		}
		else {
			auto hr = f_dev->D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
			assert(hr == S_OK);

			hr = f_dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&global));
			assert(hr == S_OK);
		}

		globalSub.pDesc = &global;
		globalSub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
		subObj.push_back(globalSub);
	}
	/*ローカルルートシグネチャの生成*/
	std::vector<ID3D12RootSignature*>local(_countof(exportName));
	std::vector<D3D12_STATE_SUBOBJECT>localSub(local.size());
	std::vector<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION>association(localSub.size() + 1);
	std::vector<D3D12_STATE_SUBOBJECT>associationSub(association.size());
	{
		D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
		desc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
		desc.Version        = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

		Microsoft::WRL::ComPtr<ID3DBlob>sig = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob>error = nullptr;
		for (std::uint32_t i = 0; i < local.size(); ++i) {
			if (f_dev->UsingRaytracingDriver()) {
				auto hr = D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
				assert(hr == S_OK);

				hr = dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&local[i]));
				assert(hr == S_OK);
			}
			else {
				auto hr = f_dev->D3D12SerializeVersionedRootSignature(&desc, &sig, &error);
				assert(hr == S_OK);

				hr = f_dev->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&local[i]));
				assert(hr == S_OK);
			}

			localSub[i].pDesc = &local[i];
			localSub[i].Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
			subObj.push_back(localSub[i]);

			association[i].NumExports = 1;
			association[i].pExports = &exportName[i];
			association[i].pSubobjectToAssociate = &(*subObj.rbegin());

			associationSub[i].pDesc = &association[i];
			associationSub[i].Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
			subObj.push_back(associationSub[i]);
		}
	}
	/*シェーダコンフィグの設定*/
	const std::uint32_t payloadSize = sizeof(float) * 3;
	D3D12_RAYTRACING_SHADER_CONFIG sConfig{};
	D3D12_STATE_SUBOBJECT sConfigSub{};
	{
		sConfig.MaxAttributeSizeInBytes = sizeof(float) * 2;
		sConfig.MaxPayloadSizeInBytes = payloadSize;

		sConfigSub.pDesc = &sConfig;
		sConfigSub.Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
		subObj.push_back(sConfigSub);

		association.rbegin()->NumExports = _countof(exportName);
		association.rbegin()->pExports = exportName;
		association.rbegin()->pSubobjectToAssociate = &(*subObj.rbegin());

		associationSub.rbegin()->pDesc = &(*association.rbegin());
		associationSub.rbegin()->Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
		subObj.push_back(*associationSub.rbegin());
	}
	/*パイプラインコンフィグの設定*/
	const std::uint32_t rayDepth = 1;
	D3D12_RAYTRACING_PIPELINE_CONFIG pConfig{};
	D3D12_STATE_SUBOBJECT pConfigSub{};
	{
		pConfig.MaxTraceRecursionDepth = rayDepth;

		pConfigSub.pDesc = &pConfig;
		pConfigSub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
		subObj.push_back(pConfigSub);
	}
	/*パイプラインの生成*/
	ID3D12StateObject* pipe = nullptr;
	ID3D12RaytracingFallbackStateObject* f_pipe = nullptr;
	{
		D3D12_STATE_OBJECT_DESC desc{};
		desc.NumSubobjects = std::uint32_t(subObj.size());
		desc.pSubobjects   = subObj.data();
		desc.Type          = D3D12_STATE_OBJECT_TYPE::D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;

		if (f_dev->UsingRaytracingDriver()) {
			auto hr = dev->CreateStateObject(&desc, IID_PPV_ARGS(&pipe));
			assert(hr == S_OK);
		}
		else {
			auto hr = f_dev->CreateStateObject(&desc, IID_PPV_ARGS(&f_pipe));
			assert(hr == S_OK);
		}
	}
	/*プリミティブの生成*/
	ID3D12Resource1* primitive = nullptr;
	{
		/*頂点データ*/
		const float vertex[] = {
			 0.0f  ,  1.0f, 0.0f,
			 0.866f, -0.5f, 0.0f,
			-0.866f, -0.5f, 0.0f
		};
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
			desc.Width            = sizeof(vertex[0]) * _countof(vertex);

			auto hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, nullptr, IID_PPV_ARGS(&primitive));
			assert(hr == S_OK);
		}
		{
			void* buf = nullptr;
			auto hr = primitive->Map(0, nullptr, &buf);
			assert(hr == S_OK);
			std::memcpy(buf, vertex, primitive->GetDesc().Width);
			primitive->Unmap(0, nullptr);
		}
	}
	/*加速構造(ボトムレベル)の生成*/
	ID3D12DescriptorHeap* bottomHeap = nullptr;
	ID3D12Resource1* bottom = nullptr;
	{
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			desc.NodeMask       = 0;
			desc.NumDescriptors = 1;
			desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

			auto hr = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&bottomHeap));
			assert(hr == S_OK);
		}

		D3D12_RAYTRACING_GEOMETRY_DESC geo{};
		geo.Flags                                = D3D12_RAYTRACING_GEOMETRY_FLAGS::D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
		geo.Triangles.VertexBuffer.StartAddress  = primitive->GetGPUVirtualAddress();
		geo.Triangles.VertexBuffer.StrideInBytes = sizeof(float) * 3;
		geo.Triangles.VertexCount                = std::uint32_t(primitive->GetDesc().Width / geo.Triangles.VertexBuffer.StrideInBytes);
		geo.Triangles.VertexFormat               = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		geo.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE::D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
		input.DescsLayout    = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
		input.Flags          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
		input.NumDescs       = 1;
		input.pGeometryDescs = &geo;
		input.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
		if (f_dev->UsingRaytracingDriver() == true) {
			dev->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);
		}
		else {
			f_dev->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);
		}

		Microsoft::WRL::ComPtr<ID3D12Resource1>scratch = nullptr;
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

		auto hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, nullptr, IID_PPV_ARGS(&scratch));
		assert(hr == S_OK);

		desc.Width = info.ResultDataMaxSizeInBytes;
		if (f_dev->UsingRaytracingDriver() == true) {
			hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, nullptr, IID_PPV_ARGS(&bottom));
		}
		else {
			hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				f_dev->GetAccelerationStructureResourceState(), nullptr, nullptr, IID_PPV_ARGS(&bottom));
		}
		assert(hr == S_OK);
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.Buffer.Flags       = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_RAW;
			desc.Buffer.NumElements = std::uint32_t(bottom->GetDesc().Width / sizeof(std::uint32_t));
			desc.Format             = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
			desc.ViewDimension      = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

			dev->CreateUnorderedAccessView(bottom, nullptr, &desc, bottomHeap->GetCPUDescriptorHandleForHeapStart());
		}
		{
			allo->Reset();
			list->Reset(allo, nullptr);

			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
			desc.DestAccelerationStructureData    = bottom->GetGPUVirtualAddress();
			desc.Inputs                           = input;
			desc.ScratchAccelerationStructureData = scratch->GetGPUVirtualAddress();
			if (f_dev->UsingRaytracingDriver() == true) {
				list->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
			}
			else {
				f_list->SetDescriptorHeaps(1, (ID3D12DescriptorHeap* const*)&bottomHeap);
				f_list->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
			}
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Flags         = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
			barrier.UAV.pResource = bottom;
			list->ResourceBarrier(1, &barrier);

			list->Close();
			queue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&list);
			{
				hr = queue->Signal(fence, ++fenceCnt);
				assert(hr == S_OK);
				
				if (fence->GetCompletedValue() != fenceCnt) {
					void* handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
					hr = fence->SetEventOnCompletion(fenceCnt, handle);
					assert(hr == S_OK);

					WaitForSingleObjectEx(handle, INFINITE, 0);
					CloseHandle(handle);
				}
			}
		}
	}
	/*加速構造(トップレベル)の生成*/
	ID3D12DescriptorHeap* topHeap = nullptr;
	ID3D12Resource1* top = nullptr;
	WRAPPED_GPU_POINTER topAdr = {};
	{
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			desc.NodeMask       = 0;
			desc.NumDescriptors = 1;
			desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

			auto hr = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&topHeap));
			assert(hr == S_OK);
		}

		Microsoft::WRL::ComPtr<ID3D12Resource1>instance = nullptr;

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

		auto hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, nullptr, IID_PPV_ARGS(&instance));
		assert(hr == S_OK);

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input{};
		input.DescsLayout   = D3D12_ELEMENTS_LAYOUT::D3D12_ELEMENTS_LAYOUT_ARRAY;
		input.Flags         = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
		input.InstanceDescs = instance->GetGPUVirtualAddress();
		input.NumDescs      = 1;
		input.Type          = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE::D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;

		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info{};
		if (f_dev->UsingRaytracingDriver() == true) {
			dev->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);
		}
		else {
			f_dev->GetRaytracingAccelerationStructurePrebuildInfo(&input, &info);
		}

		Microsoft::WRL::ComPtr<ID3D12Resource1>scratch = nullptr;
		prop.Type  = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.Width = info.ScratchDataSizeInBytes;

		hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, nullptr, IID_PPV_ARGS(&scratch));
		assert(hr == S_OK);

		desc.Width = info.ResultDataMaxSizeInBytes;
		if (f_dev->UsingRaytracingDriver() == true) {
			hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, nullptr, IID_PPV_ARGS(&top));
		}
		else {
			hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				f_dev->GetAccelerationStructureResourceState(), nullptr, nullptr, IID_PPV_ARGS(&top));
		}
		assert(hr == S_OK);
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.Buffer.Flags       = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_RAW;
			desc.Buffer.NumElements = std::uint32_t(top->GetDesc().Width / sizeof(std::uint32_t));
			desc.Format             = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
			desc.ViewDimension      = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

			dev->CreateUnorderedAccessView(top, nullptr, &desc, topHeap->GetCPUDescriptorHandleForHeapStart());
		}
		{
			D3D12_RAYTRACING_FALLBACK_INSTANCE_DESC* buf = nullptr;
			hr = instance->Map(0, nullptr, (void**)&buf);
			assert(hr == S_OK);

			buf->AccelerationStructure               = f_dev->GetWrappedPointerSimple(0, bottom->GetGPUVirtualAddress());
			buf->Flags                               = D3D12_RAYTRACING_INSTANCE_FLAGS::D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
			buf->InstanceContributionToHitGroupIndex = 0;
			buf->InstanceID                          = 0;
			buf->InstanceMask                        = 0xff;
			for (std::uint32_t i = 0; i < 3; ++i) {
				buf->Transform[i][i] = 1.0f;
			}

			instance->Unmap(0, nullptr);
		}
		{
			allo->Reset();
			list->Reset(allo, nullptr);

			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc{};
			desc.DestAccelerationStructureData    = top->GetGPUVirtualAddress();
			desc.Inputs                           = input;
			desc.ScratchAccelerationStructureData = scratch->GetGPUVirtualAddress();
			if (f_dev->UsingRaytracingDriver() == true) {
				list->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
			}
			else {
				f_list->SetDescriptorHeaps(1, (ID3D12DescriptorHeap* const*)&topHeap);
				f_list->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
			}
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Flags         = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
			barrier.UAV.pResource = top;
			list->ResourceBarrier(1, &barrier);

			list->Close();
			queue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&list);
			{
				hr = queue->Signal(fence, ++fenceCnt);
				assert(hr == S_OK);

				if (fence->GetCompletedValue() != fenceCnt) {
					void* handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
					hr = fence->SetEventOnCompletion(fenceCnt, handle);
					assert(hr == S_OK);

					WaitForSingleObjectEx(handle, INFINITE, 0);
					CloseHandle(handle);
				}
			}
		}

		topAdr = f_dev->GetWrappedPointerSimple(0, top->GetGPUVirtualAddress());
	}
	/*レイトレーシング結果関連*/
	ID3D12DescriptorHeap* resultHeap = nullptr;
	ID3D12Resource1* resultRsc = nullptr;
	{
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			desc.NodeMask       = 0;
			desc.NumDescriptors = 2;
			desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

			auto hr = dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&resultHeap));
			assert(hr == S_OK);
		}
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
			desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
			desc.Format           = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.Height           = sizeY;
			desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.MipLevels        = 1;
			desc.SampleDesc       = { 1, 0 };
			desc.Width            = sizeX;

			auto hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, nullptr, IID_PPV_ARGS(&resultRsc));
			assert(hr == S_OK);
		}
		auto handle = resultHeap->GetCPUDescriptorHandleForHeapStart();
		{
			D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
			desc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;

			dev->CreateUnorderedAccessView(resultRsc, nullptr, &desc, handle);
		}
		{
			if (f_dev->UsingRaytracingDriver() == true) {
				D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
				desc.RaytracingAccelerationStructure.Location = top->GetGPUVirtualAddress();
				desc.Shader4ComponentMapping                  = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				desc.ViewDimension                            = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;

				handle.ptr += dev->GetDescriptorHandleIncrementSize(resultHeap->GetDesc().Type);
				dev->CreateShaderResourceView(nullptr, &desc, handle);
			}
		}
	}
	/*シェーダテーブルの生成*/
	std::vector<ID3D12Resource1*>shaderTbl(_countof(exportName));
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
		desc.Width            = (((D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + 8) + D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT - 1) * D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT) / D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT;

		for (std::uint32_t i = 0; i < shaderTbl.size(); ++i) {
			auto hr = dev->CreateCommittedResource1(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, nullptr, IID_PPV_ARGS(&shaderTbl[i]));
			assert(hr == S_OK);

			std::uint8_t* buf = nullptr;
			hr = shaderTbl[i]->Map(0, nullptr, (void**)&buf);
			assert(hr == S_OK);

			void* data = nullptr;
			if (f_dev->UsingRaytracingDriver() == true) {
				Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>property = nullptr;
				hr = pipe->QueryInterface(IID_PPV_ARGS(&property));
				assert(hr == S_OK);

				data = property->GetShaderIdentifier(exportName[i]);
			}
			else {
				data = f_pipe->GetShaderIdentifier(exportName[i]);
			}
			std::memcpy(buf, data, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
			shaderTbl[i]->Unmap(0, nullptr);
		}
	}

	while (CheckMsg()) {
		allo->Reset();
		list->Reset(allo, nullptr);
		{
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource   = resultRsc;
			barrier.Transition.StateAfter  = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			list->ResourceBarrier(1, &barrier);
		}

		if (f_dev->UsingRaytracingDriver() == true) {
			list->SetPipelineState1(pipe);
		}
		else {
			list->SetComputeRootSignature(global);
			ID3D12DescriptorHeap* heap[] = {
				resultHeap,
			};
			f_list->SetDescriptorHeaps(_countof(heap), heap);
			list->SetComputeRootDescriptorTable(0, resultHeap->GetGPUDescriptorHandleForHeapStart());
			list->SetComputeRootShaderResourceView(1, top->GetGPUVirtualAddress());
			f_list->SetTopLevelAccelerationStructure(1, topAdr);
			f_list->SetPipelineState1(f_pipe);
		}

		D3D12_DISPATCH_RAYS_DESC desc{};
		desc.Depth  = 1;
		desc.Height = sizeY;
		desc.Width  = sizeX;

		desc.RayGenerationShaderRecord.SizeInBytes  = shaderTbl[0]->GetDesc().Width;
		desc.RayGenerationShaderRecord.StartAddress = shaderTbl[0]->GetGPUVirtualAddress();

		desc.MissShaderTable.SizeInBytes   = shaderTbl[1]->GetDesc().Width;
		desc.MissShaderTable.StartAddress  = shaderTbl[1]->GetGPUVirtualAddress();
		desc.MissShaderTable.StrideInBytes = shaderTbl[1]->GetDesc().Width;

		desc.HitGroupTable.SizeInBytes   = shaderTbl[2]->GetDesc().Width;
		desc.HitGroupTable.StartAddress  = shaderTbl[2]->GetGPUVirtualAddress();
		desc.HitGroupTable.StrideInBytes = shaderTbl[2]->GetDesc().Width;

		if (f_dev->UsingRaytracingDriver() == true) {
			list->DispatchRays(&desc);
		}
		else {
			f_list->DispatchRays(&desc);
		}
		{
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource   = resultRsc;
			barrier.Transition.StateAfter  = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			list->ResourceBarrier(1, &barrier);
		}
		{
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource   = rtvRsc[swap->GetCurrentBackBufferIndex()];
			barrier.Transition.StateAfter  = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			list->ResourceBarrier(1, &barrier);
		}
		list->CopyResource(rtvRsc[swap->GetCurrentBackBufferIndex()], resultRsc);
		{
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource   = rtvRsc[swap->GetCurrentBackBufferIndex()];
			barrier.Transition.StateAfter  = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			list->ResourceBarrier(1, &barrier);
		}
		list->Close();
		queue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&list);
		{
			auto hr = queue->Signal(fence, ++fenceCnt);
			assert(hr == S_OK);

			if (fence->GetCompletedValue() != fenceCnt) {
				void* handle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
				hr = fence->SetEventOnCompletion(fenceCnt, handle);
				assert(hr == S_OK);

				WaitForSingleObjectEx(handle, INFINITE, 0);
				CloseHandle(handle);
			}

			swap->Present(1, 0);
		}
	}

	return 0;
}
