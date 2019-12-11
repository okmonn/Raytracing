#pragma once

struct ID3D12RootSignature;
class SubObj;

class Root
{
public:
	// �R���X�g���N�^
	Root(const SubObj* sub);
	// �f�X�g���N�^
	~Root();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const;

private:
	// ���[�g�V�O�l�`���̐���
	void CreateRoot(const SubObj* sub);


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;
};
