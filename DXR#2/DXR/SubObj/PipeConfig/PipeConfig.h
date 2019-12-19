#pragma once
#include "../SubObj.h"

struct D3D12_RAYTRACING_PIPELINE_CONFIG;

class PipeConfig :
	public SubObj
{
public:
	// �R���X�g���N�^
	PipeConfig(const size_t& traceNum);
	// �f�X�g���N�^
	~PipeConfig();

private:
	// �T�u�I�u�W�F�N�g�̐ݒ�
	void SetSubObj(const size_t& traceNum);


	// �p�C�v���C���R���t�B�O
	std::unique_ptr<D3D12_RAYTRACING_PIPELINE_CONFIG>config;
};
