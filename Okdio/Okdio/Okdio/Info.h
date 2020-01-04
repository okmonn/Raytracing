#pragma once
#include <string>

namespace okmonn
{
	// �I�[�f�B�I�f�o�C�X�^�C�v
	enum class AudioDevType
	{
		//�o��
		output,
		//����
		input
	};

	// �I�[�f�B�I�f�o�C�X���
	struct AudioDevProp
	{
		//ID
		std::string id;
		//�t���l�[��
		std::string fullName;
		//�V���[�g�l�[��
		std::string shortName;
		//�����f�o�C�X��
		std::string devName;
	};

	// �I�[�f�B�I�Đ��^�C�v
	enum class PlayType
	{
		//���L���[�h
		shared,
		//�r�����[�h
		exclusive
	};

	// �T�E���h���
	struct SoundInfo
	{
		//�T���v�����O���g��
		unsigned int sample;
		//�ʎq���r�b�g��(�o�C�g�\�L)
		unsigned char byte;
		//�`�����l����
		unsigned char channel;
		//32bit-float����t���O
		unsigned char flag;

		SoundInfo(const unsigned int& s = 0, const unsigned char& b = 0, const unsigned char& c = 0, const unsigned char& f = 0) {
			sample  = s;
			byte    = b;
			channel = c;
			flag    = f;
		}
		SoundInfo(const SoundInfo& info) {
			(*this) = info;
		}

		void operator=(const SoundInfo& info) {
			sample  = info.sample;
			byte    = info.byte;
			channel = info.channel;
			flag    = info.flag;
		}
	};
}
