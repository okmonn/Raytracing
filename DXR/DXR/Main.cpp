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
#include "DXR/SubObj/SubObj.h"
#include "DXR/Root/Root.h"
#include "DXR/Pipe/Pipe.h"
#include <d3d12.h>

// 三角形の頂点データ
const Vec3f triVertex[] = {
	Vec3f(0,          1,  0),
	Vec3f(0.866f,  -0.5f, 0),
	Vec3f(-0.866f, -0.5f, 0),
};

int main()
{
	SubObj global("DXR/Shader/GlobalInfo.hlsl", "lib_6_3", { "global", "sConfig", "pConfig" });
	Root root(&global);
	Pipe pipe({ &global });

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
	
	while (Window::CheckMsg())
	{

	}

	return 0;
}