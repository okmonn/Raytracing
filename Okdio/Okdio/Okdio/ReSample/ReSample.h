#pragma once
#include "../Info.h"
#include <wrl.h>
#include <vector>

struct IMFMediaType;
struct IMFTransform;

class ReSample
{
public:
	// コンストラクタ
	ReSample(const float& quality = 1.0f);
	// デストラクタ
	~ReSample();

	// リサンプル
	std::vector<float> Convert(const std::vector<float>& data, const okmonn::SoundInfo& input, const okmonn::SoundInfo& output);

private:
	// 入力メディアの初期化
	void InitInputType(void);

	// 出力メディアの初期化
	void InitOutputType(void);

	// 開始
	void Start(const float& quality);

	// 入力メディアの設定
	void SetInputType(const okmonn::SoundInfo& info);

	// 出力メディアの設定
	void SetOutputType(const okmonn::SoundInfo& info);

	// 開始メッセージの送信
	void SendStartMsg(void);

	// 入力データの設定
	void SetInputData(void* desc, const std::vector<float>& input);

	// 出力データの取得
	void GetOutputData(void* desc, std::vector<float>& output);

	// 残りのデータ取得メッセージの送信
	void SendRemainingData(void);


	// 入力メディア
	Microsoft::WRL::ComPtr<IMFMediaType>inputType;

	// 出力メディア
	Microsoft::WRL::ComPtr<IMFMediaType>outputType;

	// 変換インターフェイス
	Microsoft::WRL::ComPtr<IMFTransform>transform;
};
