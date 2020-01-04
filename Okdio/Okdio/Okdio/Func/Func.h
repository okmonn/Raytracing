#pragma once
#include "../Info.h"
#include <vector>

namespace okmonn
{
	// ���j�R�[�h��������}���`�o�C�g�����ɕϊ�
	std::string ChangeCode(const std::wstring& str);

	// �I�[�f�B�I�f�o�C�X�̗�
	std::vector<AudioDevProp> GetAudioDevProp(const AudioDevType& type);

	// �T�E���h���̎擾
	okmonn::SoundInfo GetInfo(const std::string& fileName);
}
