#pragma once
#include "../Info.h"
#include <vector>

namespace okmonn
{
	// ���j�R�[�h��������}���`�o�C�g�����ɕϊ�
	std::string ChangeCode(const std::wstring& str);
	// string�ɕϊ�
	std::string ChangeCode(const char* str, const size_t& size);

	// �I�[�f�B�I�f�o�C�X�̗�
	std::vector<AudioDevProp> GetAudioDevProp(const AudioDevType& type);
}
