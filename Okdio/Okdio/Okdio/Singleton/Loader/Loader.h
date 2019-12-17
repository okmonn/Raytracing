#pragma once
#include "../Singleton.h"
#include "../../Info.h"
#include <vector>
#include <memory>
#include <unordered_map>

class Loader :
	public Singleton<Loader>
{
	friend Singleton<Loader>;

	// �ǂݍ��݃f�[�^
	struct LoadData
	{
		//�T�E���h���
		okmonn::SoundInfo info;
		//�g�`�f�[�^
		std::shared_ptr<std::vector<float>>wave;
	};

public:
	// �ǂݍ���
	long Load(const std::string& fileName);

	// �ǂݍ��݃f�[�^�̍폜
	long Delete(const std::string& fileName);

	// �T�E���h���̎擾
	okmonn::SoundInfo Info(const std::string& fileName);

	// �g�`�f�[�^�̎擾
	std::shared_ptr<std::vector<float>> Wave(const std::string& fileName);

private:
	// �R���X�g���N�^
	Loader();
	// �f�X�g���N
	~Loader();


	// �ǂݍ��݃f�[�^
	std::unordered_map<std::string, LoadData>sound;
};
