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

	// 読み込みデータ
	struct LoadData
	{
		//サウンド情報
		okmonn::SoundInfo info;
		//波形データ
		std::shared_ptr<std::vector<float>>wave;
	};

public:
	// 読み込み
	long Load(const std::string& fileName);

	// 読み込みデータの削除
	long Delete(const std::string& fileName);

	// サウンド情報の取得
	okmonn::SoundInfo Info(const std::string& fileName);

	// 波形データの取得
	std::shared_ptr<std::vector<float>> Wave(const std::string& fileName);

private:
	// コンストラクタ
	Loader();
	// デストラク
	~Loader();


	// 読み込みデータ
	std::unordered_map<std::string, LoadData>sound;
};
