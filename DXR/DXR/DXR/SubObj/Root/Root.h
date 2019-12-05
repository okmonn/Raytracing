#pragma once
#include "../SubObj.h"

struct D3D12_ROOT_SIGNATURE_DESC;
struct ID3D12RootSignature;
class Shader;

class Root :
	public SubObj
{
public:
	/*�R���X�g���N�^
	���[�J�����[�g�V�O�l�`��*/
	Root(const D3D12_ROOT_SIGNATURE_DESC& desc);
	/*�R���X�g���N�^
	�O���[�o�����[�g�V�O�l�`��*/
	Root();
	// �f�X�g���N�^
	~Root();

	// ���C�W�F�l���[�V�����p�\���̂̎擾
	static D3D12_ROOT_SIGNATURE_DESC RayGenDesc(void);

	// �~�X�p�\���̂̎擾
	static D3D12_ROOT_SIGNATURE_DESC MissDesc(void);

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const;

private:
	// ���[�J�����[�g�V�O�l�`���̐���
	void CreateLocal(const D3D12_ROOT_SIGNATURE_DESC& desc);

	// �O���[�o�����[�g�V�O�l�`���̐���
	void CreateGlobal(void);


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;
};
