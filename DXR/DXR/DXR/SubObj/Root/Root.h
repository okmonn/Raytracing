#pragma once
#include "../SubObj.h"

struct ID3D12RootSignature;
class Shader;

class Root :
	public SubObj
{
public:
	/*�R���X�g���N�^
	���[�J�����[�g�V�O�l�`��*/
	Root(const Shader* shader);
	/*�R���X�g���N�^
	�O���[�o�����[�g�V�O�l�`��*/
	Root();
	// �f�X�g���N�^
	~Root();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const;

private:
	// ���[�J�����[�g�V�O�l�`���̐���
	void CreateLocal(void);

	// ���[�J�����[�g�V�O�l�`���̐���
	void CreateLocal(const Shader* shader);

	// �O���[�o�����[�g�V�O�l�`���̐���
	void CreateGlobal(void);


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;
};
