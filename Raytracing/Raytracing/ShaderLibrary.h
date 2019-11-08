#pragma once
#include <string>
#include <vector>
#include <d3d12.h>
#include <unordered_map>

struct IDxcBlob;

class ShaderLibrary
{
	struct Library
	{
		//���C�u�����f�X�N
		D3D12_DXIL_LIBRARY_DESC library;
		//�X�e�[�^�X�I�u�W�F�N�g
		D3D12_STATE_SUBOBJECT state;
		//�֐���
		std::vector<std::wstring>funcName;
		//�֐����
		std::vector<D3D12_EXPORT_DESC>desc;
		IDxcBlob* blob;

		Library() {
			library = {};
			state   = {};
		}
	};

public:
	// �C���X�^���X�ϐ��擾
	static ShaderLibrary& Get(void);

	// �V�F�[�_�R���p�C��
	void Compile(const std::wstring& fileName, const std::initializer_list<std::wstring>& entry, const std::wstring& ver);

	// �X�e�[�^�X�I�u�W�F�N�g�擾
	D3D12_STATE_SUBOBJECT GetSubObject(const std::wstring& fileName);

	// �V�F�[�_�o�C�g�R�[�h�擾
	D3D12_SHADER_BYTECODE GetByteCode(const std::wstring& fileName);

private:
	// �R���X�g���N�^
	ShaderLibrary();
	// �f�X�g���N
	~ShaderLibrary();

	// �V�F�[�_�R���p�C��
	void Compile(const std::wstring& fileName, const std::wstring& ver, IDxcBlob** blob);


	// �V�F�[�_���
	std::unordered_map<std::wstring, Library>hlsl;
};
