#include "Okdio/OkdioEngine.h"
#include "Okdio/Okdio.h"

// エントリーポイント
int main()
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	OkdioEngine engine(okmonn::AudioDevType::output, okmonn::PlayType::shared);
	engine.Start();
	Okdio* okdio = nullptr;
	engine.CreateOkdio("SOS.wav", IID_PPV_ARGS(&okdio));

	okdio->Play(true);
	while (true)
	{

	}

	okdio->Release();
	engine.Release();
	CoUninitialize();

	return 0;
}
