#pragma once
#include <string>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

namespace DXR
{
	// �}���`�o�C�g�������烆�j�R�[�h�����ɕϊ�
	std::wstring ChangeCode(const std::string& str);
	// �}���`�o�C�g�������烆�j�R�[�h�����ɕϊ�
	wchar_t* ChangeCode(const char* str);

	// �V�F�[�_�̃R���p�C��
	void Compile(const std::string& fileName, const std::string& entry, const std::string& ver, ID3DBlob** blob);
}
