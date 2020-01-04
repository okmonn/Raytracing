#pragma once
#include "../Func/Func.h"
#include <memory>
#include <fstream>

namespace wav
{
	// RIFF�`�����N
	struct RIFF
	{
		//ID
		char id[4];
		//�f�[�^�S�̂̃T�C�Y
		long size;
		//�f�[�^�^�C�v
		char type[4];
	};
	
	// FMT�`�����N
	struct FMT
	{
		//ID
		char id[4];
		//�`�����N���T�C�Y
		long size;
		//�`�����N�^�C�v
		short type;
		//�`�����l��
		short channel;
		//�T���v�����O���g��
		long sample;
		//1�b�Ԃ̃o�C�g��
		long byte;
		//1�T���v���̃o�C�g��
		short block;
		//�ʎq���r�b�g��
		short bit;
	};

	long Load(const std::string& fileName, std::shared_ptr<std::vector<float>>& data, okmonn::SoundInfo& info)
	{
		std::ifstream file(fileName.c_str(), std::ios::binary);
		//�t�@�C���I�[�v������
		if (!file)
		{
			return 1;
		}

		//RIFF�`�����N�̓ǂݍ���
		RIFF riff{};
		file.read((char*)&riff, sizeof(riff));
		if (std::string(riff.id, sizeof(riff.id)) != "RIFF"
			|| std::string(riff.type, sizeof(riff.type)) != "WAVE")
		{
			return 1;
		}
		
		//FMT�`�����N�̓ǂݍ���
		FMT fmt{};
		file.read((char*)&fmt, sizeof(fmt));
		if (std::string(fmt.id, sizeof(fmt.id)) != "fmt ")
		{
			return 1;
		}
		//FMT�`�����N�̊g�������X�L�b�v
		file.seekg(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)), std::ios::cur);

		//DATA�`�����N�܂ŃX�L�b�v
		long size = 0;
		std::string id = "data";
		do
		{
			file.seekg(size, std::ios::cur);
			file.read(&id[0], id.size());
			file.read((char*)&size, sizeof(size));
		} while (id != "data");

		info = okmonn::SoundInfo(fmt.sample, fmt.bit / 8, unsigned char(fmt.channel), (fmt.type == 3) ? 1 : 0);

		//DATA�`�����N�̓ǂݍ���
		switch (info.byte)
		{
		case 1:
		{
			std::vector<unsigned char>tmp(size / info.byte);
			file.read((char*)tmp.data(), sizeof(tmp[0]) * tmp.size());
			data = std::make_shared<std::vector<float>>(tmp.begin(), tmp.end());
			for (float& i : *data)
			{
				i = (i / (0xff - 1) / 2) - 1.0f;
			}
			return 0;
		}
		case 2:
		{
			std::vector<short>tmp(size / info.byte);
			file.read((char*)tmp.data(), sizeof(tmp[0]) * tmp.size());
			data = std::make_shared<std::vector<float>>(tmp.begin(), tmp.end());
			for (float& i : *data)
			{
				i /= (0xffff - 1) / 2;
			}
			return 0;
		}
		default:
			break;
		}

		return 1;
	}
}
