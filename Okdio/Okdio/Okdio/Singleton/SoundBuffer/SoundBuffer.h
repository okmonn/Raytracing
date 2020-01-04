#pragma once
#include "../Singleton.h"
#include <wrl.h>
#include <mutex>
#include <vector>
#include <memory>

class SoundBuffer :
	public Singleton<SoundBuffer>
{
	friend Singleton<SoundBuffer>;
public:
	// �T�E���h�o�b�t�@�T�C�Y�̕ύX
	void ReSize(const size_t& num);

	// �T�E���h�o�b�t�@�ɒǉ�
	void Submit(const std::vector<float>& wave);

	// �T�E���h�o�b�t�@�̃N���A
	void Clear(void);

	// �T�E���h�o�b�t�@�̎擾
	std::shared_ptr<std::vector<float>> GetBuffer(void) const;

private:
	// �R���X�g���N�^
	SoundBuffer();
	// �f�X�g���N�^
	~SoundBuffer();


	// �r������
	std::mutex mtx;

	// �T�E���h�o�b�t�@
	std::shared_ptr<std::vector<float>>buf;
};
