#pragma once
#include <Unknwn.h>

template <typename T>
class UnKnown
{
public:
	// �Q�ƃJ�E���g���Z
	unsigned long __stdcall AddRef(void) {
		return InterlockedIncrement(&cnt);
	}

	// �Q�ƃJ�E���g����
	unsigned long __stdcall Release(void) {
		unsigned long hr = InterlockedDecrement(&cnt);
		if (hr == 0)
		{
			delete this;
		}

		return hr;
	}

	// �T�|�[�g�`�F�b�N
	long __stdcall QueryInterface(const GUID& id, void** obj) {
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}

		*obj = nullptr;
		if (id == __uuidof(T))
		{
			*obj = (void*)this;
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

private:
	// �Q�ƃJ�E���g
	long cnt;

protected:
	// �R���X�g���N�^
	UnKnown() : cnt(1) {}
	// �f�X�g���N
	virtual ~UnKnown() {}
};
