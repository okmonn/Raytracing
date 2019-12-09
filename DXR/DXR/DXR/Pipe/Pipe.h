#pragma once
#include <initializer_list>

struct ID3D12StateObject;
class Shader;

class Pipe
{
public:
	// �R���X�g���N�^
	Pipe(const std::initializer_list<Shader*>& obj);
	// �f�X�g���N�^
	~Pipe();

	// �p�C�v���C���̎擾
	ID3D12StateObject* Get(void) const;

private:
	// �p�C�v���C���̐���
	void CreatePipe(const std::initializer_list<Shader*>& obj);


	// �p�C�v���C��
	ID3D12StateObject* pipe;
};
