#include "DXR/Window/Window.h"
#include "DXR/Device/Device.h"
#include "DXR/Queue/Queue.h"
#include "DXR/Allocator/Allocator.h"
#include "DXR/List/List.h"
#include "DXR/Fence/Fence.h"
#include "DXR/Swap/Swap.h"
#include "DXR/Descriptor/Render/Render.h"
#include "DXR/Descriptor/Primitive/Primitive.h"
#include "DXR/Descriptor/Acceleration/Acceleration.h"
#include "DXR/Descriptor/Output/Output.h"
#include "DXR/SubObj/SubObj.h"
#include "DXR/Root/Root.h"
#include "DXR/Pipe/Pipe.h"
#include "DXR/Descriptor/ShaderTbl/ShaderTbl.h"
#include <d3d12.h>

// 三角形の頂点データ
const Vec3f triVertex[] = {
	Vec3f(0,          1,  0),
	Vec3f(0.866f,  -0.5f, 0),
	Vec3f(-0.866f, -0.5f, 0),
};

int main()
{
	Window win(Vec2(640, 480));
	Queue queue(DXR::CommandType::DIRECT);
	Allocator allo(DXR::CommandType::DIRECT);
	List list(DXR::CommandType::DIRECT);
	Fence fence(&queue);
	Swap swap(&win, &queue);
	Render render(&swap);
	Primitive triangle(triVertex, _countof(triVertex));

	allo.Reset();
	list.Reset(&allo);
	Acceleration bottom(&list, &triangle);
	Acceleration top(&list, &bottom, 1);
	list.Close();
	ID3D12CommandList* tmp[] = {
		list.Get()
	};
	queue.Execution(tmp);
	fence.Wait();

	Output output(&win, &top);

	SubObj global("DXR/Shader/GlobalInfo.hlsl", "lib_6_3", { "global", "sConfig", "pConfig" });
	SubObj raygen("DXR/Shader/RayGen.hlsl", "lib_6_3", { "local", "association", "RayGen" });
	SubObj miss("DXR/Shader/Miss.hlsl", "lib_6_3", { "local", "association", "Miss" });
	SubObj hit("DXR/Shader/TriangleHit.hlsl", "lib_6_3", { "local", "hit", "association", "TriangleHit" });
	Root root(&global);
	Pipe pipe({ &global, &raygen, &miss, &hit });

	ShaderTbl rayTbl(&pipe);
	rayTbl.RayGeneration("RayGen", &output);
	ShaderTbl missTbl(&pipe);
	missTbl.Miss("Miss");
	ShaderTbl closest(&pipe);
	closest.Closest("TriangleHit");
	
	while (Window::CheckMsg())
	{
		allo.Reset();
		list.Reset(&allo);

		top.UpData();

		list.SetRoot(&root);
		list.SetPipe(&pipe);

		list.Barrier(output.Rsc(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		D3D12_DISPATCH_RAYS_DESC desc{};
		desc.Depth = 1;
		desc.Height = win.WinSize().y;
		desc.Width = win.WinSize().x;
		
		desc.RayGenerationShaderRecord.SizeInBytes  = ShaderTbl::EntrySize();
		desc.RayGenerationShaderRecord.StartAddress = rayTbl.Rsc()->GetGPUVirtualAddress();

		desc.MissShaderTable.SizeInBytes   = ShaderTbl::EntrySize();
		desc.MissShaderTable.StartAddress  = missTbl.Rsc()->GetGPUVirtualAddress();
		desc.MissShaderTable.StrideInBytes = ShaderTbl::EntrySize();

		desc.HitGroupTable.SizeInBytes   = ShaderTbl::EntrySize();
		desc.HitGroupTable.StartAddress  = closest.Rsc()->GetGPUVirtualAddress();
		desc.HitGroupTable.StrideInBytes = ShaderTbl::EntrySize();
	}

	return 0;
}