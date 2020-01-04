#include "Okdio.h"
#include "ReSample/ReSample.h"
#include "Singleton/Loader/Loader.h"
#include "Singleton/OkdioList/OkdioList.h"
#include "Singleton/SoundBuffer/SoundBuffer.h"

// �R���X�g���N�^
Okdio::Okdio(const std::string& fileName) : 
	resample(std::make_unique<ReSample>()), read(0), play(false), loop(false)
{
	if (SUCCEEDED(Loader::Get().Load(fileName)))
	{
		name = fileName;
	}
}

// �f�X�g���N�^
Okdio::~Okdio()
{
	OkdioList::Get().Delete(this);
}

// �Đ�
void Okdio::Play(const bool& loop)
{
	play = true;
	this->loop = loop;
}

// ��~
void Okdio::Stop(void)
{
	play = false;
}

// �Q�ƃt�@�C�����̎擾
std::string Okdio::GetName(void) const
{
	return name;
}

// �g�`�f�[�^���T�E���h�o�b�t�@�ɒǉ�
void Okdio::Submit(void)
{
	if (play == true)
	{
		std::weak_ptr<std::vector<float>>wave = Loader::Get().Wave(name);
		size_t num = SoundBuffer::Get().GetBuffer()->size() * (float(Loader::Get().Info(name).sample) / float(info.sample));

		if (read + num <= Loader::Get().Wave(name)->size())
		{
			SoundBuffer::Get().Submit(resample->Convert(std::vector<float>(&wave.lock()->at(read), &wave.lock()->at(read + num)), Loader::Get().Info(name), info));
			//SoundBuffer::Get().Submit(std::vector<float>(&wave.lock()->at(read), &wave.lock()->at(read + num)));
			read += num;
		}
		else
		{
			num = (read + num) - Loader::Get().Wave(name)->size();
			SoundBuffer::Get().Submit(resample->Convert(std::vector<float>(&wave.lock()->at(read), &wave.lock()->at(read + num)), Loader::Get().Info(name), info));
			if (loop == false)
			{
				Stop();
			}

			read = 0;
		}
	}
}


// �T�E���h���̎擾
okmonn::SoundInfo Okdio::GetInfo(void) const
{
	return info;
}

// �T�E���h���̐ݒ�
void Okdio::SetInfo(const okmonn::SoundInfo& info)
{
	this->info = info;
}
