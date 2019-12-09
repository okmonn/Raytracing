#pragma once
#include "../SubObj.h"

struct ID3D12RootSignature;
class Shader;

class Root :
	public SubObj
{
public:
	// �R���X�g���N�^
	Root(const Shader* shader);
	// �f�X�g���N�^
	~Root();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const;

private:
	// ���[�g�V�O�l�`���̐���
	void CreateRoot(const Shader* shader);


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;
};
