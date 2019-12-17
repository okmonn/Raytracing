#include "ReSample.h"
#include <wrl.h>
#include <mfapi.h>
#include <mftransform.h>
#include <Mferror.h>
#include <wmcodecdsp.h>
#include <ks.h>
#include <ksmedia.h>

#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "wmcodecdspuuid")

// �X�s�[�J�[�ꗗ
const unsigned long spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

// �R���X�g���N�^
ReSample::ReSample() :
	inputType(nullptr), outputType(nullptr), transform(nullptr)
{
	InitInputType();
	InitOutputType();
}

// �f�X�g���N�^
ReSample::~ReSample()
{
}

// ���̓��f�B�A�̏�����
void ReSample::InitInputType(void)
{
	auto hr = MFCreateMediaType(&inputType);
	_ASSERT(hr == S_OK);

	hr = inputType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	_ASSERT(hr == S_OK);
	hr = inputType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_Float);
	_ASSERT(hr == S_OK);
	hr = inputType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, sizeof(float) * 8);
	_ASSERT(hr == S_OK);
	hr = inputType->SetUINT32(MF_MT_AUDIO_PREFER_WAVEFORMATEX, true);
	_ASSERT(hr == S_OK);
	hr = inputType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, true);
	_ASSERT(hr == S_OK);
}

// �o�̓��f�B�A�̏�����
void ReSample::InitOutputType(void)
{
	auto hr = MFCreateMediaType(&outputType);
	_ASSERT(hr == S_OK);

	//���̓��f�B�A�̓��e�R�s�[
	hr = inputType->CopyAllItems(outputType);
	_ASSERT(hr == S_OK);
}

// �J�n
void ReSample::Start(const float& quality)
{
	auto hr = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr<IUnknown>unknown = nullptr;
	hr = CoCreateInstance(CLSID_CResamplerMediaObject, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&unknown));
	_ASSERT(hr == S_OK);

	hr = unknown->QueryInterface(IID_PPV_ARGS(&transform));
	_ASSERT(hr == S_OK);

	Microsoft::WRL::ComPtr< IWMResamplerProps>prop = nullptr;
	hr = unknown->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	//�i����ݒ�
	hr = prop->SetHalfFilterLength(long(60.0f * quality));
	_ASSERT(hr == S_OK);
}

// ���̓��f�B�A�̐ݒ�
void ReSample::SetInputType(const okmonn::SoundInfo& info)
{
	auto hr = inputType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, info.channel);
	_ASSERT(hr == S_OK);
	hr = inputType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, info.sample);
	_ASSERT(hr == S_OK);
	hr = inputType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, sizeof(float) * info.channel);
	_ASSERT(hr == S_OK);
	hr = inputType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, info.sample * info.channel * sizeof(float));
	_ASSERT(hr == S_OK);
	hr = transform->SetInputType(0, inputType, 0);
	_ASSERT(hr == S_OK);
}

// �o�̓��f�B�A�̐ݒ�
void ReSample::SetOutputType(const okmonn::SoundInfo& info)
{
	auto hr = outputType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, info.channel);
	_ASSERT(hr == S_OK);
	hr = outputType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, info.sample);
	_ASSERT(hr == S_OK);
	hr = outputType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, sizeof(float) * info.channel);
	_ASSERT(hr == S_OK);
	hr = outputType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, info.sample * info.channel * sizeof(float));
	_ASSERT(hr == S_OK);
	hr = transform->SetOutputType(0, outputType, 0);
	_ASSERT(hr == S_OK);
}

// �J�n���b�Z�[�W�̑��M
void ReSample::SendStartMsg(void)
{
	auto hr = transform->ProcessMessage(MFT_MESSAGE_COMMAND_FLUSH, 0);
	_ASSERT(hr == S_OK);
	hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, 0);
	_ASSERT(hr == S_OK);
	hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_START_OF_STREAM, 0);
	_ASSERT(hr == S_OK);

}

// ���̓f�[�^�̐ݒ�
void ReSample::SetInputData(void* desc, const std::vector<float>& input)
{
	Microsoft::WRL::ComPtr<IMFMediaBuffer>buf = nullptr;
	auto hr = MFCreateMemoryBuffer(sizeof(float) * input.size(), &buf);
	_ASSERT(hr == S_OK);

	//�f�[�^�̃R�s�[
	unsigned char* tmp = nullptr;
	hr = buf->Lock(&tmp, nullptr, nullptr);
	_ASSERT(hr == S_OK);
	std::memcpy(tmp, input.data(), sizeof(float) * input.size());
	hr = buf->Unlock();
	_ASSERT(hr == S_OK);

	//�L���͈͂̐ݒ�
	hr = buf->SetCurrentLength(sizeof(float) * input.size());
	_ASSERT(hr == S_OK);

	//�o�b�t�@�̒ǉ�
	hr = ((MFT_OUTPUT_DATA_BUFFER*)desc)->pSample->AddBuffer(buf.Get());
	_ASSERT(hr == S_OK);

	//�T���v���[�̃Z�b�g
	hr = transform->ProcessInput(0, ((MFT_OUTPUT_DATA_BUFFER*)desc)->pSample, 0);
	_ASSERT(hr == S_OK);
}

// �o�̓f�[�^�̎擾
void ReSample::GetOutputData(void* desc, std::vector<float>& output)
{
	std::vector<float>data;
	while (true)
	{
		auto hr = transform->ProcessOutput(0, 1, (MFT_OUTPUT_DATA_BUFFER*)desc, &((MFT_OUTPUT_DATA_BUFFER*)desc)->dwStatus);
		if (hr == MF_E_TRANSFORM_NEED_MORE_INPUT)
		{
			break;
		}

		Microsoft::WRL::ComPtr<IMFMediaBuffer>buf = nullptr;
		hr = ((MFT_OUTPUT_DATA_BUFFER*)desc)->pSample->ConvertToContiguousBuffer(&buf);
		_ASSERT(hr == S_OK);

		//�L���͈͂̎擾
		unsigned long size = 0;
		hr = buf->GetCurrentLength(&size);
		_ASSERT(hr == S_OK);

		//�z��̃T�C�Y�ݒ�
		data.resize(size / sizeof(float));

		//�f�[�^�̃R�s�[
		unsigned char* tmp = nullptr;
		hr = buf->Lock(&tmp, nullptr, nullptr);
		_ASSERT(hr == S_OK);
		std::memcpy(data.data(), tmp, size);
		hr = buf->Unlock();
		_ASSERT(hr == S_OK);

		//�o�̓f�[�^�ɒǉ�
		output.insert(output.end(), data.begin(), data.end());
	}
}

// �c��̃f�[�^�擾���b�Z�[�W�̑��M
void ReSample::SendRemainingData(void)
{
	auto hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_END_OF_STREAM, 0);
	_ASSERT(hr == S_OK);
	hr = transform->ProcessMessage(MFT_MESSAGE_COMMAND_DRAIN, 0);
	_ASSERT(hr == S_OK);
}

// ���T���v��
std::vector<float> ReSample::Convert(const std::vector<float>& data, const okmonn::SoundInfo& input, const okmonn::SoundInfo& output)
{
	SetInputType(input);
	SetOutputType(output);
	SendStartMsg();

	MFT_OUTPUT_DATA_BUFFER desc{};
	auto hr = MFCreateSample(&desc.pSample);
	_ASSERT(hr == S_OK);

	SetInputData((void*)&desc, data);

	std::vector<float>tmp;
	GetOutputData((void*)&desc, tmp);
	SendRemainingData();
	GetOutputData((void*)&desc, tmp);

	return tmp;
}

// �I��
void ReSample::Finish(void)
{
	auto hr = transform->ProcessMessage(MFT_MESSAGE_NOTIFY_END_STREAMING, 0);
	_ASSERT(hr == S_OK);

	if (outputType != nullptr)
	{
		outputType->Release();
		outputType = nullptr;
	}
	if (inputType != nullptr)
	{
		inputType->Release();
		inputType = nullptr;
	}
	if (transform != nullptr)
	{
		transform->Release();
		transform = nullptr;
	}

	hr = MFShutdown();
	_ASSERT(hr == S_OK);
}
