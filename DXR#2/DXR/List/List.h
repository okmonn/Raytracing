#pragma once
#include "../Info.h"

enum D3D12_RESOURCE_STATES : int;
struct ID3D12GraphicsCommandList5;
struct ID3D12Resource;
class Window;
class Allocator;
class Pipe;
class Root;

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

	// �r���[�|�[�g�̃Z�b�g
	void Viewport(const Window* win);

	// �V�U�[�̃Z�b�g
	void Scissor(const Window* win);

	// �p�C�v���C���̃Z�b�g
	void SetPipe(const Pipe* pipe);

	// ���[�g�V�O�l�`���̃Z�b�g
	void SetRoot(const Root* root);

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList5* Get(void) const;

private:
	// �R�}���h���X�g�̐���
	void CreateList(const DXR::CommandType& type);


	// �R�}���h���X�g
	ID3D12GraphicsCommandList5* list;
};
