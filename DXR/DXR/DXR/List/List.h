#pragma once
#include "../Information.h"

struct ID3D12GraphicsCommandList5;
struct ID3D12Resource;

class List
{
public:
	// �R���X�g���N�^
	List(const DXR::CommandType& type);
	// �f�X�g���N
	~List();

	// UAV�o���A
	void Barrier(ID3D12Resource* rsc) const;

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList5* Get(void) const;
	
private:
	// �R�}���h���X�g�̐���
	void CreateList(const DXR::CommandType& type);


	// �R�}���h���X�g
	ID3D12GraphicsCommandList5* list;
};
