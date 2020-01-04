#pragma once
#include "../Info.h"
#include <vector>

namespace okmonn
{
	// ユニコード文字からマルチバイト文字に変換
	std::string ChangeCode(const std::wstring& str);

	// オーディオデバイスの列挙
	std::vector<AudioDevProp> GetAudioDevProp(const AudioDevType& type);

	// サウンド情報の取得
	okmonn::SoundInfo GetInfo(const std::string& fileName);
}
