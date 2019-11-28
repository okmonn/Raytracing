#pragma once

struct ID3D12Device5;

class Device
{
public:
	// �f�o�C�X�̎擾
	static ID3D12Device5* Get(void);

private:
	Device(const Device&)         = delete;
	void operator=(const Device&) = delete;

	// �R���X�g���N�^
	Device();
	// �f�X�g���N
	~Device();

	// �f�o�C�X�̐���
	void CreateDevice(void);


	// �f�o�C�X
	ID3D12Device5* device;
};
