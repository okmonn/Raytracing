#include "Window/Window.h"
#include "Allocator/Allocator.h"
#include "List/List.h"
#include "Queue/Queue.h"
#include "Fence/Fence.h"
#include "Swap/Swap.h"
#include "Descriptor/Render/Render.h"
#include "Descriptor/Primitive/Primitive.h"
#include "Descriptor/Acceleration/Acceleration.h"
#include "SubObj/SubObj.h"
#include "Pipe/Pipe.h"
#include "Root/Root.h"
#include "Descriptor/Output/Output.h"
#include "Descriptor/ShaderTbl/ShaderTbl.h"
#include <d3d12.h>

int main()
{
	Window win(Vec2(640, 480));
	Allocator allo(DXR::CommandType::DIRECT);
	List list(DXR::CommandType::DIRECT);
	Queue queue(DXR::CommandType::DIRECT);
	Fence fence(&queue);
	Swap swap(&win, &queue);
	Render render(&swap);

	Vec3f vertex[] = {
		Vec3f(0,          1,  0),
		Vec3f(0.866f,  -0.5f, 0),
		Vec3f(-0.866f, -0.5f, 0),
	};
	Primitive triangle(vertex, _countof(vertex));

	allo.Reset();
	list.Reset(&allo);
	Acceleration bottom(&list, &triangle);
	Acceleration top(&list, { &bottom });
	list.Close();
	queue.Execution(&list, 1);
	fence.Wait();

	SubObj sub("Shader/test.hlsl", "lib_6_4", 
	   {"global", "rayGen", "rayGenAsso", 
		"miss", "missAsso", 
		"closest", "hit", "closestAsso",
		"sConfig", "pConfig", 
		"RayGen", "Miss", "Chs"});

	Pipe pipe({&sub});

	Root root(&sub);

	Output output(&win, &top);

	ShaderTbl raygen(&pipe);
	raygen.RayGeneration(&output, {"RayGen"});
	/*ShaderTbl miss(&pipe);
	miss.Miss({ "Miss" });
	ShaderTbl closest(&pipe);
	closest.ClosestHit({"hit"});*/

	float color[] = {
		0.4f, 0.6f, 0.2f, 1.0f
	};
	while (Window::CheckMsg())
	{
		allo.Reset();
		list.Reset(&allo);

		/*list.Viewport(&win);
		list.Scissor(&win);*/

		list.SetPipe(&pipe);
		list.SetRoot(&root);

		list.Barrier(output.Rsc(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		D3D12_DISPATCH_RAYS_DESC desc{};
		desc.Depth = 1;
		desc.Height = win.WinSize().y;
		desc.Width = win.WinSize().x;

		desc.RayGenerationShaderRecord.SizeInBytes = ShaderTbl::EntrySize();
		desc.RayGenerationShaderRecord.StartAddress = raygen.Rsc()->GetGPUVirtualAddress();

		desc.MissShaderTable.SizeInBytes = ShaderTbl::EntrySize();
		desc.MissShaderTable.StartAddress = raygen.Rsc()->GetGPUVirtualAddress() + ShaderTbl::EntrySize();
		desc.MissShaderTable.StrideInBytes = ShaderTbl::EntrySize();

		desc.HitGroupTable.SizeInBytes = ShaderTbl::EntrySize();
		desc.HitGroupTable.StartAddress = raygen.Rsc()->GetGPUVirtualAddress() + ShaderTbl::EntrySize() * 2;
		desc.HitGroupTable.StrideInBytes = ShaderTbl::EntrySize();
		
		list.Get()->DispatchRays(&desc);
		list.Barrier(output.Rsc(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);

		/*list.Barrier(render.Rsc(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
		render.Clear(&list, color);
		list.Barrier(render.Rsc(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);*/

		list.Barrier(render.Rsc(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
		list.Get()->CopyResource(render.Rsc(), output.Rsc());
		list.Barrier(render.Rsc(), D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

		list.Close();
		queue.Execution(&list, 1);
		fence.Wait();
		swap.Present();
	}

	return 0;
}