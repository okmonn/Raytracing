#include "OkdioEngine.h"
#include "Okdio.h"
#include "Singleton/OkdioList/OkdioList.h"
#include "Singleton/SoundBuffer/SoundBuffer.h"
#include <ks.h>
#include <wrl.h>
#include <cmath>
#include <memory>
#include <ksmedia.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

// スピーカー一覧
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

// コンストラクタ
OkdioEngine::OkdioEngine(const okmonn::AudioDevType& devType, const okmonn::PlayType& playType, const unsigned int& devIndex, const okmonn::SoundInfo* info) : 
	devType(devType), playType(playType)
{
	for (auto& i : handle)
	{
		i = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
	}

	CreateDevice(devIndex);
	CreateAudioClient();
	Initialized(info);
	CreateRender();

	if (th.joinable() == false)
	{
		th = std::thread(&OkdioEngine::AudioThread, this);
	}
}

// デストラクタ
OkdioEngine::~OkdioEngine()
{
	if (th.joinable() == true)
	{
		SetEvent(*handle.rbegin());
		th.join();
	}

	for (auto& i : handle)
	{
		CloseHandle(i);
		i = nullptr;
	}
}

// デバイスの生成
void OkdioEngine::CreateDevice(const unsigned int& index)
{
	//オーディオデバイス列挙
	Microsoft::WRL::ComPtr<IMMDeviceEnumerator>enumerator = nullptr;
	auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&enumerator));
	_ASSERT(hr == S_OK);

	//オーディオデバイスコレクション
	Microsoft::WRL::ComPtr<IMMDeviceCollection>collection = nullptr;
	hr = enumerator->EnumAudioEndpoints(EDataFlow(devType), DEVICE_STATE_ACTIVE, &collection);
	_ASSERT(hr == S_OK);

	//デバイス数
	unsigned int devNum = 0;
	hr = collection->GetCount(&devNum);
	_ASSERT(hr == S_OK);

	//デフォルトデバイス使用
	if (index >= devNum)
	{
		auto hr = enumerator->GetDefaultAudioEndpoint(EDataFlow(devType), ERole::eConsole, &device);
	}
	//指定デバイス使用
	else
	{
		auto hr = collection->Item(index, &device);
	}
	_ASSERT(hr == S_OK);
}

// オーディオクライアントの生成
void OkdioEngine::CreateAudioClient(void)
{
	auto hr = device->Activate(__uuidof(IAudioClient3), CLSCTX_INPROC_SERVER, nullptr, (void**)&audio);
	_ASSERT(hr == S_OK);
}

// フォーマットの確認
void* OkdioEngine::CheckFormat(void)
{
	WAVEFORMATEXTENSIBLE* fmt = nullptr;
	//共有モード
	if (playType == okmonn::PlayType::shared)
	{
		auto hr = audio->GetMixFormat((WAVEFORMATEX**)&fmt);
		_ASSERT(hr == S_OK);
	}
	//排他モード
	else
	{
		Microsoft::WRL::ComPtr<IPropertyStore>store = nullptr;
		auto hr = device->OpenPropertyStore(STGM_READ, &store);
		_ASSERT(hr == S_OK);

		PROPVARIANT variant{};
		hr = store->GetValue(PKEY_AudioEngine_DeviceFormat, &variant);
		_ASSERT(hr == S_OK);

		fmt = (WAVEFORMATEXTENSIBLE*)variant.blob.pBlobData;
	}

	return fmt;
}

// オーディオクライアントの初期化
void OkdioEngine::Initialized(const okmonn::SoundInfo* info)
{
	//フォーマットのチェック
	WAVEFORMATEXTENSIBLE* corre = nullptr;
	std::unique_ptr<WAVEFORMATEXTENSIBLE>fmt = std::make_unique<WAVEFORMATEXTENSIBLE>();
	if (info == nullptr)
	{
		*fmt = *(WAVEFORMATEXTENSIBLE*)CheckFormat();

		this->info.sample  = fmt->Format.nSamplesPerSec;
		this->info.byte    = fmt->Format.wBitsPerSample / 8;
		this->info.channel = unsigned char(fmt->Format.nChannels);
		this->info.flag    = (fmt->SubFormat == KSDATAFORMAT_SUBTYPE_PCM) ? 0 : 1;
	}
	else
	{
		fmt->Format.cbSize               = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
		fmt->Format.nChannels            = info->channel;
		fmt->Format.nSamplesPerSec       = info->sample;
		fmt->Format.wBitsPerSample       = info->byte * 8;
		fmt->Format.nBlockAlign          = info->byte * info->channel;
		fmt->Format.nAvgBytesPerSec      = info->sample * info->byte * info->channel;
		fmt->Format.wFormatTag           = WAVE_FORMAT_EXTENSIBLE;
		fmt->dwChannelMask               = spk[info->channel - 1];
		fmt->Samples.wValidBitsPerSample = unsigned short(fmt->Format.nSamplesPerSec);
		fmt->SubFormat                   = (info->flag == 0) ? KSDATAFORMAT_SUBTYPE_PCM : KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

		this->info = *info;
	}

	auto hr = audio->IsFormatSupported(AUDCLNT_SHAREMODE(playType), (WAVEFORMATEX*)&(*fmt), (WAVEFORMATEX**)&corre);
	if (FAILED(hr))
	{
		hr = audio->IsFormatSupported(AUDCLNT_SHAREMODE(playType), (WAVEFORMATEX*)corre, nullptr);
		_ASSERT(hr == S_OK);

		this->info.byte    = corre->Format.wBitsPerSample / 8;
		this->info.channel = unsigned char(corre->Format.nChannels);
		this->info.flag    = (corre->SubFormat == KSDATAFORMAT_SUBTYPE_PCM) ? 0 : 1;
		this->info.sample  = corre->Format.nSamplesPerSec;
	}

	//レイテンシ取得
	REFERENCE_TIME def = 0;
	REFERENCE_TIME min = 0;
	hr = audio->GetDevicePeriod(&def, &min);
	_ASSERT(hr == S_OK);

	//初期化
	hr = audio->Initialize(AUDCLNT_SHAREMODE(playType), AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST,
		def, def * int(playType), (WAVEFORMATEX*)&(*fmt), nullptr);
	//再挑戦
	if (hr == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED)
	{
		//フレーム取得
		unsigned __int32 frame = 0;
		hr = audio->GetBufferSize(&frame);
		_ASSERT(hr == S_OK);

		def = REFERENCE_TIME(std::round(10000.0 * 1000.0 * frame / fmt->Format.nSamplesPerSec));
		hr = audio->Initialize(AUDCLNT_SHAREMODE(playType), AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST,
			def, def * int(playType), (WAVEFORMATEX*)&(*fmt), nullptr);
	}
	_ASSERT(hr == S_OK);

	hr = audio->SetEventHandle(*handle.begin());
	_ASSERT(hr == S_OK);
}

// オーディオレンダラーの生成
void OkdioEngine::CreateRender(void)
{
	auto hr = audio->GetService(IID_PPV_ARGS(&render));
	_ASSERT(hr == S_OK);

	//初期バッファのクリア
	unsigned __int32 fream = 0;
	hr = audio->GetBufferSize(&fream);
	_ASSERT(hr == S_OK);
	unsigned char* buf = nullptr;
	hr = render->GetBuffer(fream, &buf);
	_ASSERT(hr == S_OK);
	hr = render->ReleaseBuffer(fream, AUDCLNT_BUFFERFLAGS_SILENT);
	_ASSERT(hr == S_OK);
}

// オーディオスレッド処理
void OkdioEngine::AudioThread(void)
{
	unsigned __int32 fream = 0;
	auto hr = audio->GetBufferSize(&fream);
	_ASSERT(hr == S_OK);

	unsigned __int32 padding = 0;
	unsigned char* buf = nullptr;

	while (true)
	{
		hr = WaitForMultipleObjects(handle.size(), handle.data(), false, INFINITE);
		if (hr != WAIT_OBJECT_0)
		{
			break;
		}

		//共有モードのみ
		if (playType == okmonn::PlayType::shared)
		{
			hr = audio->GetCurrentPadding(&padding);
			_ASSERT(hr == S_OK);
		}

		std::unique_lock<std::mutex>lock(mtx);
		hr = render->GetBuffer(fream - padding, &buf);
		_ASSERT(hr == S_OK);
		SoundBuffer::Get().ReSize((fream - padding) * info.channel);
		for (auto& i : OkdioList::Get().Get().GetList())
		{
			i->Submit();
		}
		std::memcpy(buf, SoundBuffer::Get().GetBuffer()->data(), (fream - padding) * info.channel * info.byte);
		hr = render->ReleaseBuffer(fream - padding, 0);
		_ASSERT(hr == S_OK);

		SoundBuffer::Get().Clear();
	}
}

// Okdioの生成
bool OkdioEngine::CreateOkdio(const std::string& fileName, const GUID& id, void** okdio)
{
	if (id == __uuidof(Okdio))
	{
		*okdio = new Okdio(fileName);
		((Okdio*)*okdio)->SetInfo(info);
		OkdioList::Get().Add((Okdio**)okdio);
		return true;
	}

	return false;
}

// 開始
bool OkdioEngine::Start(void)
{
	auto hr = audio->Start();
	if (SUCCEEDED(hr))
	{
		return true;
	}

	return false;
}

// 停止
bool OkdioEngine::Stop(void)
{
	auto hr = audio->Stop();
	if (SUCCEEDED(hr))
	{
		return true;
	}

	return false;
}
