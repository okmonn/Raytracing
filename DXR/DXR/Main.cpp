#include "DXR/Window/Window.h"
#include "DXR/Queue/Queue.h"
#include "DXR/Allocator/Allocator.h"
#include "DXR/List/List.h"
#include "DXR/Fence/Fence.h"
#include "DXR/Swap/Swap.h"

int main()
{
	Window win(Vec2(640, 480));
	Queue queue(DXR::CommandType::DIRECT);
	Allocator allo(DXR::CommandType::DIRECT);
	List list(DXR::CommandType::DIRECT);
	Fence fence(&queue);
	Swap swap(&win, &queue);
		
	while (Window::CheckMsg())
	{

	}

	return 0;
}