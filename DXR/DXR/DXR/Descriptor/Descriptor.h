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
	// �q�[�v�̎擾
	ID3D12DescriptorHeap* Heap(void) const;

	// ���\�[�X�̎擾
	ID3D12Resource* Rsc(const size_t& index) const;

	// ���\�[�X�̎擾
	std::vector<ID3D12Resource*>Rsc(void) const;
		
private:

protected:
	// �R���X�g���N�^
	Descriptor();
	// �f�X�g���N
	virtual ~Descriptor();

	// �q�[�v�̐���
	void CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const size_t& rscNum, const bool& shader = false);

	// �f�t�H���g�v���p�e�B�̎擾
	D3D12_HEAP_PROPERTIES DefaultProp(void);

	// �A�b�v���[�h�v���p�e�B�̎擾
	D3D12_HEAP_PROPERTIES UploadProp(void);

	// ���\�[�X�̐���
	void CreateRsc(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc, const D3D12_RESOURCE_STATES& state, 
		const size_t& index = 0, const D3D12_HEAP_FLAGS& flag = D3D12_HEAP_FLAGS(0), const D3D12_CLEAR_VALUE* clear = nullptr);

	// �}�b�v
	void Map(void** buf, const size_t& index = 0);

	// �A���}�b�v
	void UnMap(const size_t& index = 0);


	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X
	std::vector<ID3D12Resource*>rsc;
};
