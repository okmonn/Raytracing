#pragma once
#include "../Descriptor.h"

struct D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS;
class List;
class Primitive;

class Acceleration :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Acceleration(const List* list, const Primitive* prim);
	// �R���X�g���N�^
	Acceleration(const List* list, const std::initializer_list<Acceleration*>& bottom);
	// �f�X�g���N�^
	~Acceleration();

	// �f�[�^�̃Z�b�g
	void UpData(const size_t& depth = 1);

	// �X�N���b�`���\�[�X�̎擾
	ID3D12Resource* Scratch(void) const;

	// ���U���g���\�[�X�̎擾
	ID3D12Resource* Result(void) const;

	// �C���X�^���X���\�[�X�̎擾
	ID3D12Resource* Instance(void) const;

private:
	// �{�g�����x���̐���
	void CreateBottom(void);

	// �g�b�v���x���̐���
	void CreateTop();

	//	�r���h
	void Build(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& input);


	// ���X�g
	const List* list;

	// �v���~�e�B�u
	const Primitive* prim;

	// �{�g��
	std::vector<Acceleration*>bottom;
};
