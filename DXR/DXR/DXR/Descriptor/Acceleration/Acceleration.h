#pragma once
#include "../Descriptor.h"
#include <vector>
#include <memory>

struct D3D12_RAYTRACING_GEOMETRY_DESC;
struct D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS;
struct D3D12_RAYTRACING_INSTANCE_DESC;
class List;
class Primitive;

class Acceleration :
	public Descriptor
{
public:
	/*�R���X�g���N�^
	�{�g�����x��*/
	Acceleration(const List* list, const Primitive* prim);
	/*�R���X�g���N�^
	�g�b�v���x��*/
	Acceleration(const List* list, const Acceleration* bottom, const size_t& bottomNum);
	// �f�X�g���N�^
	~Acceleration();

	// �g�b�v���x���̍X�V
	void UpData(const size_t& rayNum = 1);

	// ���U���g���\�[�X�̎擾
	ID3D12Resource* Result(void) const;

	// �X�N���b�`���\�[�X�̎擾
	ID3D12Resource* Scratch(void) const;

	// �C���X�^���X���\�[�X�̎擾
	ID3D12Resource* Instance(void) const;

	// �����\�����͏��̎擾
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS Input(void) const;

private:
	// �{�g�����x���̐���
	void CreateBottom(void);

	// �g�b�v���x���̐���
	void CreateTop(const Acceleration* bottom, const size_t& bottomNum);

	// �r���h
	void Build(const List* list);


	// ���X�g
	const List* list;

	// �v���~�e�B�u
	const Primitive* prim;

	// �o�b�t�@
	D3D12_RAYTRACING_INSTANCE_DESC* buf;

	// �W�I���g�����
	std::unique_ptr<D3D12_RAYTRACING_GEOMETRY_DESC>geo;

	// �����\�����͏��
	std::unique_ptr<D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS>input;

	// �{�g�����x��
	std::vector<const Acceleration*>bottom;
};
