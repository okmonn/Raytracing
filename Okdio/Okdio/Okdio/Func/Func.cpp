#include "Func.h"
#include <wrl.h>
#include <initguid.h>
#include <mmdeviceapi.h>
#include <propvarutil.h>
#include <functiondiscoverykeys_devpkey.h>

// ユニコード文字からマルチバイト文字に変換
std::string okmonn::ChangeCode(const std::wstring& str)
{
	std::vector<char>buf(WideCharToMultiByte(CP_OEMCP, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr));
	WideCharToMultiByte(CP_OEMCP, 0, str.c_str(), -1, buf.data(), buf.size(), nullptr, nullptr);

	return std::string(buf.begin(), buf.end());
}

// stringに変換
std::string okmonn::ChangeCode(const char* str, const size_t& size)
{
	return std::string(&str[0], size);
}

// オーディオデバイスの列挙
std::vector<okmonn::AudioDevProp> okmonn::GetAudioDevProp(const AudioDevType& type)
{
	//オーディオデバイス列挙インターフェースの生成
	Microsoft::WRL::ComPtr<IMMDeviceEnumerator>enumerator = nullptr;
	auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&enumerator));
	_ASSERT(hr == S_OK);

	//オーディオデバイスコレクションインターフェースの生成
	Microsoft::WRL::ComPtr<IMMDeviceCollection>collection = nullptr;
	hr = enumerator->EnumAudioEndpoints(EDataFlow(type), DEVICE_STATE_ACTIVE, &collection);
	_ASSERT(hr == S_OK);

	//オーディオデバイス数の取得
	unsigned int devNum = 0;
	hr = collection->GetCount(&devNum);
	_ASSERT(hr == S_OK);

	//それぞれのデバイス情報の取得
	std::vector<AudioDevProp>prop(devNum);
	Microsoft::WRL::ComPtr<IMMDevice>dev = nullptr;
	Microsoft::WRL::ComPtr<IPropertyStore>store = nullptr;
	LPWSTR buf = nullptr;
	PROPVARIANT variant{};
	for (unsigned int i = 0; i < devNum; ++i)
	{
		//デバイス取得
		hr = collection->Item(i, &dev);
		_ASSERT(hr == S_OK);

		//ID取得
		hr = dev->GetId(&buf);
		_ASSERT(hr == S_OK);
		prop[i].id = ChangeCode(std::wstring(buf));

		//デバイス名取得準備
		hr = dev->OpenPropertyStore(STGM_READ, &store);
		_ASSERT(hr == S_OK);

		//フルネーム取得
		hr = store->GetValue(PKEY_Device_FriendlyName, &variant);
		_ASSERT(hr == S_OK);
		hr = PropVariantToStringAlloc(variant, &buf);
		_ASSERT(hr == S_OK);
		prop[i].fullName = ChangeCode(std::wstring(buf));

		//ショートネーム取得
		hr = store->GetValue(PKEY_Device_DeviceDesc, &variant);
		_ASSERT(hr == S_OK);
		hr = PropVariantToStringAlloc(variant, &buf);
		_ASSERT(hr == S_OK);
		prop[i].shortName = ChangeCode(std::wstring(buf));

		//物理デバイス名取得
		hr = store->GetValue(PKEY_DeviceInterface_FriendlyName, &variant);
		_ASSERT(hr == S_OK);
		hr = PropVariantToStringAlloc(variant, &buf);
		_ASSERT(hr == S_OK);
		prop[i].devName = ChangeCode(std::wstring(buf));
	}

	//バッファの解放
	CoTaskMemFree(buf);

	return prop;
}
