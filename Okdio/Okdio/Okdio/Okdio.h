#pragma once
#include "Parent/Oknown.h"
#include "Info.h"
#include <memory>

class OkdioEngine;
class ReSample;

class __declspec(uuid("00000000-0917-0917-0917-000000000001"))
	Okdio :
	public Oknown<Okdio>
{
	friend OkdioEngine;
public:
	// �R���X�g���N�^
	Okdio(const std::string& fileName);
	// �f�X�g���N�^
	~Okdio();

	// �Đ�
	void Play(const bool& loop = false);

	// ��~
	void Stop(void);

	// �Q�ƃt�@�C�����̎擾
	std::string GetName(void) const;

	// �T�E���h���̎擾
	okmonn::SoundInfo GetInfo(void) const;

	// �T�E���h���̐ݒ�
	void SetInfo(const okmonn::SoundInfo& info);

private:
	// �g�`�f�[�^���T�E���h�o�b�t�@�ɒǉ�
	void Submit(void);


	// ���T���v���[
	std::unique_ptr<ReSample>resample;

	// �Q�ƃt�@�C����
	std::string name;

	// �ǂݍ��݈ʒu
	size_t read;

	// �Đ��t���O
	bool play;

	// ���[�v�t���O
	bool loop;

	// �T�E���h���
	okmonn::SoundInfo info;
};