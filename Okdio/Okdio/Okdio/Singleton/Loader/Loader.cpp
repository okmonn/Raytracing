#include "Loader.h"
#include "../../Format/Wav.h"
#include <functional>

namespace
{
	std::unordered_map<std::string, std::function<long(const std::string&, std::shared_ptr<std::vector<float>>&, okmonn::SoundInfo&)>>func =
	{
		{"wav", std::bind(&wav::Load, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)}
	};
}

// �R���X�g���N�^
Loader::Loader()
{
}

// �f�X�g���N
Loader::~Loader()
{
}

// �ǂݍ���
long Loader::Load(const std::string& fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		return 0;
	}

	std::string fmt = fileName.substr(fileName.find_last_of('.') + 1);
	if (func.find(fmt) != func.end())
	{
		return func[fmt](fileName, sound[fileName].wave, sound[fileName].info);
	}

	return 1;
}

// �ǂݍ��݃f�[�^�̍폜
long Loader::Delete(const std::string& fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
		return 0;
	}

	return 1;
}

// �T�E���h���̎擾
okmonn::SoundInfo Loader::Info(const std::string& fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		return sound[fileName].info;
	}

	return okmonn::SoundInfo();
}

// �g�`�f�[�^�̎擾
std::shared_ptr<std::vector<float>> Loader::Wave(const std::string& fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		return sound[fileName].wave;
	}

	return nullptr;
}
