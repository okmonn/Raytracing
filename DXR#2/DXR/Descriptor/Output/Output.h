#pragma once
#include "../Descriptor.h"

class Window;
class Acceleration;

class Output :
	public Descriptor
{
public:
	// コンストラクタ
	Output(const Window* win, const Acceleration* top);
	// デストラクタ
	~Output();

private:
	// リソースの生成
	void CreateRsc(const Window* win);

	// UAVの生成
	void UAV(const size_t& index);

	// SRVの生成
	void SRV(const Acceleration* top, const size_t& index);
};
