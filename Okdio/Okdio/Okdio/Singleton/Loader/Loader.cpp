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

// コンストラクタ
Loader::Loader()
{
}

// デストラク
Loader::~Loader()
{
}

// 読み込み
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

// 読み込みデータの削除
long Loader::Delete(const std::string& fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
		return 0;
	}

	return 1;
}

// サウンド情報の取得
okmonn::SoundInfo Loader::Info(const std::string& fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		return sound[fileName].info;
	}

	return okmonn::SoundInfo();
}

// 波形データの取得
std::shared_ptr<std::vector<float>> Loader::Wave(const std::string& fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		return sound[fileName].wave;
	}

	return nullptr;
}
