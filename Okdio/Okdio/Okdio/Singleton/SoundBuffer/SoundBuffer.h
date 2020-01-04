#pragma once
#include "../Singleton.h"
#include <wrl.h>
#include <mutex>
#include <vector>
#include <memory>

class SoundBuffer :
	public Singleton<SoundBuffer>
{
	friend Singleton<SoundBuffer>;
public:
	// サウンドバッファサイズの変更
	void ReSize(const size_t& num);

	// サウンドバッファに追加
	void Submit(const std::vector<float>& wave);

	// サウンドバッファのクリア
	void Clear(void);

	// サウンドバッファの取得
	std::shared_ptr<std::vector<float>> GetBuffer(void) const;

private:
	// コンストラクタ
	SoundBuffer();
	// デストラクタ
	~SoundBuffer();


	// 排他制御
	std::mutex mtx;

	// サウンドバッファ
	std::shared_ptr<std::vector<float>>buf;
};
