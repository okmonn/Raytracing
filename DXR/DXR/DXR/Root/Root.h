#pragma once
#include <string>
#include <vector>
#include <memory>

struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;
struct D3D12_DXIL_LIBRARY_DESC;
struct D3D12_STATE_SUBOBJECT;
struct D3D12_EXPORT_DESC;
struct ID3D12RootSignature;

class Root
{
public:
	/*�R���X�g���N�^
	���[�J�����[�g�V�O�l�`��*/
	Root(const std::string& fileName, const std::string& entry, const std::string& ver, const std::initializer_list<std::string>& func);
	/*�R���X�g���N�^
	�O���[�o�����[�g�V�O�l�`��*/
	Root();
	// �f�X�g���N�^
	~Root();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const;

	// �V�F�[�_�̃T�u�I�u�W�F�N�g�̎擾
	D3D12_STATE_SUBOBJECT ShaderSub(void) const;

	// ���[�g�V�O�l�`���̃T�u�I�u�W�F�N�g�̎擾
	D3D12_STATE_SUBOBJECT RootSub(void) const;

private:
	// �V�F�[�_�̃T�u�I�u�W�F�N�g�̐���
	void CreateShaderSub(const std::initializer_list<std::string>& func);

	// ���[�J�����[�g�V�O�l�`���̐���
	void CreateLocalRoot(void);

	// �O���[�o�����[�g�V�O�l�`���̐���
	void CreateGlobalRoot(void);


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;

	// �V�F�[�_���
	ID3DBlob* shader;

	// DXIL���C�u����
	std::unique_ptr<D3D12_DXIL_LIBRARY_DESC>desc;

	// �T�u�I�u�W�F�N�g
	std::vector<std::unique_ptr<D3D12_STATE_SUBOBJECT>>sub;

	// �I�u�W�F�N�g�̃G�N�X�|�[�g
	std::vector<std::unique_ptr<D3D12_EXPORT_DESC>>expo;

	// �V�F�[�_�֐���
	std::vector<std::wstring>name;
};
