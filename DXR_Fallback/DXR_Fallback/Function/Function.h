#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3D12RaytracingFallback.h>
#include <cstdint>
#include <initializer_list>

/*�E�B���h�E�̐���
.�E�B���h�E�n���h��
.����
.�c��*/
void CreateWnd(void** handle, const std::uint32_t& sizeX, const std::uint32_t& sizeY);
/*�E�B���h�E���b�Z�[�W�̊m�F
return �I���t���O*/
bool CheckMsg(void);
/*�f�o�C�X�̐���
.�f�o�C�X
.�@�\���x��*/
void CreateDevice(ID3D12Device5** dev, const D3D_FEATURE_LEVEL& level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1);
/*�t�H�[���o�b�N�f�o�C�X�̐���
.�t�H�[���o�b�N�p�f�o�C�X
.�f�o�C�X*/
void CreateDevice(ID3D12RaytracingFallbackDevice** f_dev, ID3D12Device5* dev);
/*�R�}���h�A���P�[�^�̐���
.�R�}���h�A���P�[�^
.�f�o�C�X
.�R�}���h�^�C�v*/
void CreateAllocator(ID3D12CommandAllocator** allo, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
/*�R�}���h�L���[�̐���
.�R�}���h�L���[
.�f�o�C�X
.�R�}���h�^�C�v
*/
void CreateQueue(ID3D12CommandQueue** queue, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
/*�R�}���h���X�g�̐���
.�R�}���h���X�g
.�f�o�C�X
.�R�}���h�^�C�v*/
void CreateList(ID3D12GraphicsCommandList4** list, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
/*�t�H�[���o�b�N�p�R�}���h���X�g�̐���
.�t�H�[���o�b�N�p�R�}���h���X�g
.�t�H�[���o�b�N�p�f�o�C�X
.�R�}���h���X�g*/
void CreateList(ID3D12RaytracingFallbackCommandList** f_list, ID3D12RaytracingFallbackDevice* dev, ID3D12GraphicsCommandList4* list);
/*�t�F���X�̐���
.�t�F���X
.�f�o�C�X*/
void CreateFence(ID3D12Fence1** fence, ID3D12Device5* dev, const std::uint64_t& initCnt = 0);
/*�X���b�v�`�F�C���̐���
.�X���b�v�`�F�C��
.�E�B���h�E�n���h��
.�R�}���h�L���[
.�o�b�N�o�b�t�@��*/
void CreateSwapChain(IDXGISwapChain4** swap, void* win, ID3D12CommandQueue* queue, const std::uint32_t& backBuffNum = 2);
/*�f�B�X�N���v�^�q�[�v�̐���
.�f�B�X�N���v�^�q�[�v
.�f�o�C�X
.�q�[�v�^�C�v
.���\�[�X��
.�V�F�[�_�t���O*/
void CreateHeap(ID3D12DescriptorHeap** heap, ID3D12Device5* dev, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& rscNum, const bool& shader = false);
/*���\�[�X�̐���
.���\�[�X
.�X���b�v�`�F�C��
.���\�[�X�C���f�b�N�X*/
void CreateRsc(ID3D12Resource1** rsc, IDXGISwapChain4* swap, const std::uint32_t& index);
/*�v���~�e�B�u���\�[�X�̐���
.���\�[�X
.�f�o�C�X
.1���_������̃T�C�Y
.���_��*/
void CreateRsc(ID3D12Resource1** rsc, ID3D12Device5* dev, const std::uint32_t& vertexStride, const std::uint32_t& vertexNum);
/*�����_�[�^�[�Q�b�g�r���[�̐���
.���\�[�X
.�f�o�C�X
.�f�B�X�N���v�^�q�[�v
.���\�[�X�C���f�b�N�X*/
void RTV(ID3D12Resource1* rsc, ID3D12Device5* dev, ID3D12DescriptorHeap* heap, const std::uint32_t& index);
/*�R�}���h�A���P�[�^�̃��Z�b�g
.�R�}���h�A���P�[�^*/
void Reset(ID3D12CommandAllocator* allo);
/*�R�}���h���X�g�̃��Z�b�g
.�R�}���h���X�g
.�R�}���h�A���P�[�^*/
void Reset(ID3D12GraphicsCommandList4* list, ID3D12CommandAllocator* allo);
/*�R�}���h���X�g�̃N���[�Y
.�R�}���h���X�g*/
void Close(ID3D12GraphicsCommandList4* list);
/*UAV���\�[�X�o���A
.���\�[�X
.�R�}���h���X�g*/
void Barrier(ID3D12Resource1* rsc, ID3D12GraphicsCommandList4* list);
/*���\�[�X�o���A
.���\�[�X
.�R�}���h���X�g
.�ڍs�O���
.�ڍs����*/
void Barrier(ID3D12Resource1* rsc, ID3D12GraphicsCommandList4* list, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
/*�o�b�N�o�b�t�@���̎擾
.�X���b�v�`�F�C��
return �o�b�N�o�b�t�@��*/
std::uint32_t GetBackBuffNum(IDXGISwapChain4* swap);
/*���݂̃o�b�N�o�b�t�@�C���f�b�N�X�̎擾
.�X���b�v�`�F�C��
return �o�b�N�o�b�t�@�C���f�b�N�X*/
std::uint32_t GetBuffIndex(IDXGISwapChain4* swap);
/*�f�[�^�̃R�s�[
.���\�[�X
.�f�[�^*/
void Copy(ID3D12Resource1* rsc, void* data);
/*�����_�[�^�[�Q�b�g�̃N���A
.�f�B�X�N���v�^�q�[�v
.�R�}���h���X�g
.���\�[�X�C���f�b�N�X
.�N���A�J���[[4]
.DSV�p�f�B�X�N���v�^�q�[�v*/
void Clear(ID3D12DescriptorHeap* heap, ID3D12Device5* dev, ID3D12GraphicsCommandList4* list, const std::uint32_t& index, const float* color, ID3D12DescriptorHeap* depth = nullptr);
/*�R�}���h�̎��s
.�R�}���h�L���[
.�R�}���h���X�g*/
void Execution(ID3D12CommandQueue* queue, const std::initializer_list<ID3D12GraphicsCommandList4*>& lists);
/*�R�}���h���s�̑ҋ@
.�t�F���X
.�R�}���h�L���[
.�X���b�v�`�F�C��
.�t�F���X�J�E���g*/
void Wait(ID3D12Fence1* fence, ID3D12CommandQueue* queue, IDXGISwapChain4* swap, std::uint64_t& fenceCnt);
