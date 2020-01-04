#pragma once
#include "Parent/Oknown.h"
#include "Info.h"
#include <wrl.h>
#include <array>
#include <mutex>
#include <thread>

struct IMMDevice;
struct IAudioClient3;
struct IAudioRenderClient;
struct _GUID;
typedef _GUID GUID;
class Okdio;

class __declspec(uuid("00000000-0917-0917-0917-000000000000"))
	OkdioEngine : 
	public Oknown<OkdioEngine>
{
public:
	// コンストラクタ
	OkdioEngine(const okmonn::AudioDevType& devType, const okmonn::PlayType& playType, const unsigned int& devIndex = UINT_MAX, const okmonn::SoundInfo* info = nullptr);
	// デストラクタ
	~OkdioEngine();

	// Okdioの生成
	bool CreateOkdio(const std::string& fileName, const GUID& id, void** okdio);

	// 開始
	bool Start(void);

	// 停止
	bool Stop(void);

private:
	// デバイスの生成
	void CreateDevice(const unsigned int& index);

	// オーディオクライアントの生成
	void CreateAudioClient(void);

	// フォーマットの確認
	void* CheckFormat(void);

	// オーディオクライアントの初期化
	void Initialized(const okmonn::SoundInfo* info);

	// オーディオレンダラーの生成
	void CreateRender(void);

	// オーディオスレッド処理
	void AudioThread(void);


	// オーディオデバイスタイプ
	okmonn::AudioDevType devType;

	// オーディオ再生タイプ
	okmonn::PlayType playType;

	// サウンド情報
	okmonn::SoundInfo info;

	// デバイス
	Microsoft::WRL::ComPtr<IMMDevice>device;

	// オーディオクライント
	Microsoft::WRL::ComPtr<IAudioClient3>audio;

	// オーディオレンダラー
	Microsoft::WRL::ComPtr<IAudioRenderClient>render;

	// イベント
	std::array<void*, 2>handle;

	// 排他制御
	std::mutex mtx;

	// スレッド
	std::thread th;
};
