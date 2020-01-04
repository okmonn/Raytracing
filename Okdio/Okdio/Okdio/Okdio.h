#pragma once
#include "Parent/Oknown.h"
#include "Info.h"
#include <memory>

class OkdioEngine;
class ReSample;

class __declspec(uuid("00000000-0917-0917-0917-000000000001"))
	Okdio :
	public Oknown<Okdio>
{
	friend OkdioEngine;
public:
	// コンストラクタ
	Okdio(const std::string& fileName);
	// デストラクタ
	~Okdio();

	// 再生
	void Play(const bool& loop = false);

	// 停止
	void Stop(void);

	// 参照ファイル名の取得
	std::string GetName(void) const;

	// サウンド情報の取得
	okmonn::SoundInfo GetInfo(void) const;

	// サウンド情報の設定
	void SetInfo(const okmonn::SoundInfo& info);

private:
	// 波形データをサウンドバッファに追加
	void Submit(void);


	// リサンプラー
	std::unique_ptr<ReSample>resample;

	// 参照ファイル名
	std::string name;

	// 読み込み位置
	size_t read;

	// 再生フラグ
	bool play;

	// ループフラグ
	bool loop;

	// サウンド情報
	okmonn::SoundInfo info;
};