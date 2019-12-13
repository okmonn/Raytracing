#pragma once
#include "../Information.h"

enum D3D12_RESOURCE_STATES : int;
struct ID3D12GraphicsCommandList5;
struct ID3D12Resource;
class Allocator;
class Root;
class Pipe;
class Descriptor;

class List
{
public:
	// �R���X�g���N�^
	List(const DXR::CommandType& type);
	// �f�X�g���N
	~List();

	// �R�}���h���X�g�̃��Z�b�g
	void Reset(const Allocator* allo) const;

	// �R�}���h���X�g�̃N���[�Y
	void Close(void) const;

	// �o���A
	void Barrier(ID3D12Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const;

	// UAV�o���A
	void Barrier(ID3D12Resource* rsc) const;

	// ���[�g�V�O�l�`���̃Z�b�g
	void SetRoot(const Root* root);

	// �p�C�v���C���̃Z�b�g
	void SetPipe(const Pipe* pipe);

	// �f�B�X�N���v�^�[�q�[�v�̃Z�b�g
	void SetHeap(const Descriptor* descriptor, const size_t& num);

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList5* Get(void) const;
	
private:
	// �R�}���h���X�g�̐���
	void CreateList(const DXR::CommandType& type);


	// �R�}���h���X�g
	ID3D12GraphicsCommandList5* list;
};
