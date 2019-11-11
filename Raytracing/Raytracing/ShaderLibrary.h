#pragma once
#include "Header.h"
#include <dxcapi.h>
#include <unordered_map>

class ShaderLibrary
{
public:
	// �R���p�C��
	void Compile(const std::wstring& fileName, const std::wstring& entry, const std::initializer_list<std::wstring>& func, const std::wstring& ver);

	// �V�F�[�_���擾
	ShaderInfo GetInfo(const std::wstring& fileName);

	// �C���X�^���X�ϐ��擾
	static ShaderLibrary& Get(void);

private:
	ShaderLibrary(const ShaderLibrary&) = delete;
	void operator=(const ShaderLibrary&) = delete;

	// �R���X�g���N�^
	ShaderLibrary();
	// �f�X�g���N
	~ShaderLibrary();

	// �R���p�C��
	void Compile(const std::wstring& fileName, const std::wstring& entry, const std::wstring& ver, IDxcBlob** blob);

	
	// �V�F�[�_���
	std::unordered_map<std::wstring, ShaderInfo>info;
};
