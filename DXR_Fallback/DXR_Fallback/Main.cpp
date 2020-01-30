#include "Main.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "FallbackLayer.lib")
#pragma comment(lib, "WinPixEventRuntime.lib")

namespace {
	/*クリアカラー*/
	const float color[] = {
		1.0f, 1.0f, 1.0f, 1.0f, 
	};
}

int main() {
	CreateWnd(&win, 640, 480);
	CreateDevice(&dev);
	CreateDevice(&f_dev, dev.Get());
	CreateAllocator(&allo, dev.Get());
	CreateQueue(&queue, dev.Get());
	CreateList(&list, dev.Get());
	CreateList(&f_list, f_dev.Get(), list.Get());
	CreateFence(&fence, dev.Get());
	CreateSwapChain(&swap, win, queue.Get());
	/*レンダーターゲットの生成*/
	rtvRsc.resize(GetBackBuffNum(swap.Get()));
	CreateHeap(&rtvHeap, dev.Get(), D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, std::uint32_t(rtvRsc.size()));
	for (std::uint32_t i = 0; i < rtvRsc.size(); ++i) {
		CreateRsc(&rtvRsc[i], swap.Get(), i);
		RTV(rtvRsc[i].Get(), dev.Get(), rtvHeap.Get(), i);
	}
	/*プリミティブの生成*/
	const float vertex[] = {
		-1.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		 0.0f, -1.0f, 1.0f
	};
	CreateRsc(&triangle, dev.Get(), sizeof(vertex[0]), _countof(vertex));
	Copy(triangle.Get(), (void*)vertex);
	/*加速構造(ボトムレベル)の生成*/
	CreateHeap(&bottomHeap, dev.Get(), D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2, true);
	Reset(allo.Get());
	Reset(list.Get(), allo.Get());
	CreateAcceleration(&bottomScratch, &bottomResult, f_dev.Get(), f_list.Get(), bottomHeap.Get(), triangle.Get(), _countof(vertex));
	Execution(queue.Get(), { list.Get() });
	Wait(fence.Get(), queue.Get(), swap.Get(), fenceCnt);
	/*加速構造(トップレベル)の生成*/
	CreateHeap(&topHeap, dev.Get(), D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3, true);
	Reset(allo.Get());
	Reset(list.Get(), allo.Get());
	CreateAcceleration(&topScratch, &topResult, &topInstance, f_dev.Get(), f_list.Get(), topHeap.Get());
	Execution(queue.Get(), { list.Get() });
	Wait(fence.Get(), queue.Get(), swap.Get(), fenceCnt);
	AccelerationUpDate(topInstance.Get(), bottomResult.Get());
	/*サブオブジェクト関係*/
	subObj.reserve(12);
	CreateGlobalRoot(&global, f_dev.Get(), subObj);
	exportName[0] = L"RayGen";
	exportName[1] = L"Miss";
	exportName[2] = L"ClosestHit";
	ShaderCompile(L"HLSL/Test.hlsl", L"lib_6_3", &shader, dxil, exportDesc, subObj, exportName, _countof(exportName));
	closestHitName = exportName[2];
	hitName = L"hit";
	SetHitGroup(hitDesc, subObj, closestHitName.data(), hitName.data());
	local.resize(_countof(exportName));
	association.resize(local.size() + 1);
	CreateLocalRayGenRoot(&local[0], f_dev.Get(), subObj);
	ExportAssociation(association[0], subObj, &exportName[0]);
	for (std::uint32_t i = 1; i < local.size(); ++i) {
		CreateLocalRoot(&local[i], f_dev.Get(), subObj);
		ExportAssociation(association[i], subObj, &exportName[i]);
	}
	SetShaderConfig(shaderConfig, subObj, sizeof(float));
	ExportAssociation(*association.rbegin(), subObj, exportName, _countof(exportName));
	SetPipeConfig(pipeConfig, subObj, 0);
	CreatePipe(&pipe, f_dev.Get(), subObj);
	/*シェーダテーブル*/
	shaderTbl.resize(_countof(exportName));

	while (CheckMsg()) {
		Reset(allo.Get());
		Reset(list.Get(), allo.Get());

		Barrier(rtvRsc[GetBuffIndex(swap.Get())].Get(), list.Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		Clear(rtvHeap.Get(), dev.Get(), list.Get(), GetBuffIndex(swap.Get()), color);
		Barrier(rtvRsc[GetBuffIndex(swap.Get())].Get(), list.Get(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
		
		Close(list.Get());
		Execution(queue.Get(), { list.Get() });
		Wait(fence.Get(), queue.Get(), swap.Get(), fenceCnt);
	}

	return 0;
}

