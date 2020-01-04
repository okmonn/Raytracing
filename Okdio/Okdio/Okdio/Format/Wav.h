#pragma once
#include "../Func/Func.h"
#include <memory>
#include <fstream>

namespace wav
{
	// RIFFチャンク
	struct RIFF
	{
		//ID
		char id[4];
		//データ全体のサイズ
		long size;
		//データタイプ
		char type[4];
	};
	
	// FMTチャンク
	struct FMT
	{
		//ID
		char id[4];
		//チャンク内サイズ
		long size;
		//チャンクタイプ
		short type;
		//チャンネル
		short channel;
		//サンプリング周波数
		long sample;
		//1秒間のバイト数
		long byte;
		//1サンプルのバイト数
		short block;
		//量子化ビット数
		short bit;
	};

	long Load(const std::string& fileName, std::shared_ptr<std::vector<float>>& data, okmonn::SoundInfo& info)
	{
		std::ifstream file(fileName.c_str(), std::ios::binary);
		//ファイルオープン判定
		if (!file)
		{
			return 1;
		}

		//RIFFチャンクの読み込み
		RIFF riff{};
		file.read((char*)&riff, sizeof(riff));
		if (std::string(riff.id, sizeof(riff.id)) != "RIFF"
			|| std::string(riff.type, sizeof(riff.type)) != "WAVE")
		{
			return 1;
		}
		
		//FMTチャンクの読み込み
		FMT fmt{};
		file.read((char*)&fmt, sizeof(fmt));
		if (std::string(fmt.id, sizeof(fmt.id)) != "fmt ")
		{
			return 1;
		}
		//FMTチャンクの拡張部分スキップ
		file.seekg(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)), std::ios::cur);

		//DATAチャンクまでスキップ
		long size = 0;
		std::string id = "data";
		do
		{
			file.seekg(size, std::ios::cur);
			file.read(&id[0], id.size());
			file.read((char*)&size, sizeof(size));
		} while (id != "data");

		info = okmonn::SoundInfo(fmt.sample, fmt.bit / 8, unsigned char(fmt.channel), (fmt.type == 3) ? 1 : 0);

		//DATAチャンクの読み込み
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
