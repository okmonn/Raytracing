#pragma once
#include "../Info.h"
#include <wrl.h>
#include <vector>

struct IMFMediaType;
struct IMFTransform;

class ReSample
{
public:
	// �R���X�g���N�^
	ReSample(const float& quality = 1.0f);
	// �f�X�g���N�^
	~ReSample();

	// ���T���v��
	std::vector<float> Convert(const std::vector<float>& data, const okmonn::SoundInfo& input, const okmonn::SoundInfo& output);

private:
	// ���̓��f�B�A�̏�����
	void InitInputType(void);

	// �o�̓��f�B�A�̏�����
	void InitOutputType(void);

	// �J�n
	void Start(const float& quality);

	// ���̓��f�B�A�̐ݒ�
	void SetInputType(const okmonn::SoundInfo& info);

	// �o�̓��f�B�A�̐ݒ�
	void SetOutputType(const okmonn::SoundInfo& info);

	// �J�n���b�Z�[�W�̑��M
	void SendStartMsg(void);

	// ���̓f�[�^�̐ݒ�
	void SetInputData(void* desc, const std::vector<float>& input);

	// �o�̓f�[�^�̎擾
	void GetOutputData(void* desc, std::vector<float>& output);

	// �c��̃f�[�^�擾���b�Z�[�W�̑��M
	void SendRemainingData(void);


	// ���̓��f�B�A
	Microsoft::WRL::ComPtr<IMFMediaType>inputType;

	// �o�̓��f�B�A
	Microsoft::WRL::ComPtr<IMFMediaType>outputType;

	// �ϊ��C���^�[�t�F�C�X
	Microsoft::WRL::ComPtr<IMFTransform>transform;
};
