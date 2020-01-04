#pragma once
#include "Parent/Oknown.h"
#include "Info.h"
#include <wrl.h>
#include <array>
#include <mutex>
#include <thread>

struct IMMDevice;
struct IAudioClient3;
struct IAudioRenderClient;
struct _GUID;
typedef _GUID GUID;
class Okdio;

class __declspec(uuid("00000000-0917-0917-0917-000000000000"))
	OkdioEngine : 
	public Oknown<OkdioEngine>
{
public:
	// �R���X�g���N�^
	OkdioEngine(const okmonn::AudioDevType& devType, const okmonn::PlayType& playType, const unsigned int& devIndex = UINT_MAX, const okmonn::SoundInfo* info = nullptr);
	// �f�X�g���N�^
	~OkdioEngine();

	// Okdio�̐���
	bool CreateOkdio(const std::string& fileName, const GUID& id, void** okdio);

	// �J�n
	bool Start(void);

	// ��~
	bool Stop(void);

private:
	// �f�o�C�X�̐���
	void CreateDevice(const unsigned int& index);

	// �I�[�f�B�I�N���C�A���g�̐���
	void CreateAudioClient(void);

	// �t�H�[�}�b�g�̊m�F
	void* CheckFormat(void);

	// �I�[�f�B�I�N���C�A���g�̏�����
	void Initialized(const okmonn::SoundInfo* info);

	// �I�[�f�B�I�����_���[�̐���
	void CreateRender(void);

	// �I�[�f�B�I�X���b�h����
	void AudioThread(void);


	// �I�[�f�B�I�f�o�C�X�^�C�v
	okmonn::AudioDevType devType;

	// �I�[�f�B�I�Đ��^�C�v
	okmonn::PlayType playType;

	// �T�E���h���
	okmonn::SoundInfo info;

	// �f�o�C�X
	Microsoft::WRL::ComPtr<IMMDevice>device;

	// �I�[�f�B�I�N���C���g
	Microsoft::WRL::ComPtr<IAudioClient3>audio;

	// �I�[�f�B�I�����_���[
	Microsoft::WRL::ComPtr<IAudioRenderClient>render;

	// �C�x���g
	std::array<void*, 2>handle;

	// �r������
	std::mutex mtx;

	// �X���b�h
	std::thread th;
};
