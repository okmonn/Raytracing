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
#include "DXR/SubObj/Root/Root.h"
#include "DXR/SubObj/Shader/Shader.h"
#include "DXR/SubObj/Hit/Hit.h"
#include "DXR/SubObj/Association/Association.h"
#include "DXR/SubObj/ShaderConfig/ShaderConfig.h"
#include "DXR/SubObj/PipeConfig/PipeConfig.h"
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
	Window win(Vec2(640, 480));
	Queue queue(DXR::CommandType::DIRECT);
	Allocator allo(DXR::CommandType::DIRECT);
	List list(DXR::CommandType::DIRECT);
	Fence fence(&queue);
	Swap swap(&win, &queue);
	Render render(&swap);
	Primitive triangle(triVertex, _countof(triVertex));
	Acceleration bottom(&list, &triangle);
	Acceleration top(&list, &bottom, 1);


	Shader shader("DXR/Shader/Raygeneration.hlsl", "", "lib_6_3", { "RayGen", "Miss", "Chs" });
	
	Hit hit("hit", "Chs");
	
	Root rayGen(&shader);
	Association rayAsso(&rayGen, { "RayGen" });

	Root miss(nullptr);
	Association missAsso(&miss, { "Miss" });

	ShaderConfig sConfig(sizeof(bool));
	Association configAsso(&sConfig, { "RayGen", "Miss" });

	PipeConfig pConfig(0);
	
	Root global;

	Pipe pipe({&shader, &hit, &rayGen, &rayAsso, &miss, &missAsso, &sConfig, &configAsso, &pConfig, &global});
	
	while (Window::CheckMsg())
	{

	}

	return 0;
}