#include "SoundBuffer.h"
#include <algorithm>

// コンストラクタ
SoundBuffer::SoundBuffer() : 
	buf(std::make_shared<std::vector<float>>())
{
}

// デストラクタ
SoundBuffer::~SoundBuffer()
{
}

// サウンドデータサイズの変更
void SoundBuffer::ReSize(const size_t& num)
{
	buf->resize(num);
}

// サウンドデータの追加
void SoundBuffer::Submit(const std::vector<float>& wave)
{
	std::unique_lock<std::mutex>lock(mtx);
	std::transform(wave.begin(), wave.end(), buf->begin(), buf->begin(), std::plus<float>());
}

// サウンドバッファのクリア
void SoundBuffer::Clear(void)
{
	buf->clear();
	buf->shrink_to_fit();
}

// サウンドバッファの取得
std::shared_ptr<std::vector<float>> SoundBuffer::GetBuffer(void) const
{
	return buf;
}
