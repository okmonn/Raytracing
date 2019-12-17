#pragma once
#include "../Info.h"
#include <vector>

namespace okmonn
{
	// ユニコード文字からマルチバイト文字に変換
	std::string ChangeCode(const std::wstring& str);
	// stringに変換
	std::string ChangeCode(const char* str, const size_t& size);

	// オーディオデバイスの列挙
	std::vector<AudioDevProp> GetAudioDevProp(const AudioDevType& type);
}
