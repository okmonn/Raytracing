#pragma once
#include <initializer_list>

struct ID3D12StateObject;
class SubObj;

class Pipe
{
public:
	// �R���X�g���N�^
	Pipe(const std::initializer_list<SubObj*>& obj);
	// �f�X�g���N�^
	~Pipe();

	// �p�C�v���C���̎擾
	ID3D12StateObject* Get(void) const;

private:
	// �p�C�v���C���̐���
	void CreatePipe(const std::initializer_list<SubObj*>& obj);


	// �p�C�v���C��
	ID3D12StateObject* pipe;
};
