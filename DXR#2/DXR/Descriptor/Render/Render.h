#pragma once
#include "../Descriptor.h"

class List;
class Swap;

class Render :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Render(const Swap* swap);
	// �f�X�g���N�^
	~Render();

	// �N���A
	void Clear(const List* list, const float* color, ID3D12DescriptorHeap* depth = nullptr);

	// ���\�[�X�̎擾
	ID3D12Resource* Rsc(void) const;

private:
	// �����_�[���\�[�X�̐���
	void CreateRenderRsc(void);

	// RTV�̐���
	void RTV(const size_t& index);


	// �X���b�v
	const Swap* swap;
};
