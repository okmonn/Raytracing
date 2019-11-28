#pragma once
#include <vector>

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
enum D3D12_HEAP_FLAGS : int;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct D3D12_HEAP_PROPERTIES;
struct D3D12_RESOURCE_DESC;
struct D3D12_CLEAR_VALUE;

class Descriptor
{
public:
	// �R���X�g���N�^
	Descriptor();
	// �f�X�g���N
	virtual ~Descriptor();

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* Heap(void) const;

	// ���\�[�X�̎擾
	ID3D12Resource* Rsc(const size_t& index) const;

	// ���\�[�X�̎擾
	std::vector<ID3D12Resource*>Rsc(void) const;
		
private:

protected:
	// �q�[�v�̐���
	void CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const size_t& rscNum, const bool& shader = false);

	// ���\�[�X�̐���
	void CreateRsc(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc, const D3D12_RESOURCE_STATES& state, 
		const size_t& index, const D3D12_HEAP_FLAGS& flag = D3D12_HEAP_FLAGS(0), const D3D12_CLEAR_VALUE* clear = nullptr);


	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X
	std::vector<ID3D12Resource*>rsc;
};
