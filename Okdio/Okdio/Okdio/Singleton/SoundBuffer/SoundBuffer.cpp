#include "SoundBuffer.h"
#include <algorithm>

// �R���X�g���N�^
SoundBuffer::SoundBuffer() : 
	buf(std::make_shared<std::vector<float>>())
{
}

// �f�X�g���N�^
SoundBuffer::~SoundBuffer()
{
}

// �T�E���h�f�[�^�T�C�Y�̕ύX
void SoundBuffer::ReSize(const size_t& num)
{
	buf->resize(num);
}

// �T�E���h�f�[�^�̒ǉ�
void SoundBuffer::Submit(const std::vector<float>& wave)
{
	std::unique_lock<std::mutex>lock(mtx);
	std::transform(wave.begin(), wave.end(), buf->begin(), buf->begin(), std::plus<float>());
}

// �T�E���h�o�b�t�@�̃N���A
void SoundBuffer::Clear(void)
{
	buf->clear();
	buf->shrink_to_fit();
}

// �T�E���h�o�b�t�@�̎擾
std::shared_ptr<std::vector<float>> SoundBuffer::GetBuffer(void) const
{
	return buf;
}
