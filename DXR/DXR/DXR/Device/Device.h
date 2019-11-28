#pragma once

struct ID3D12Device5;

class Device
{
public:
	// デバイスの取得
	static ID3D12Device5* Get(void);

private:
	Device(const Device&)         = delete;
	void operator=(const Device&) = delete;

	// コンストラクタ
	Device();
	// デストラク
	~Device();

	// デバイスの生成
	void CreateDevice(void);


	// デバイス
	ID3D12Device5* device;
};
