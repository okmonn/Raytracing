#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <dxgi1_6.h>
#include <D3D12RaytracingFallback.h>

#include <string>
#include <vector>
#include <cstdint>

/*�E�B���h�E�̐���
.�E�B���h�E�n���h��
.����
.�c��*/
void CreateWnd(void** handle, const std::uint32_t& sizeX, const std::uint32_t& sizeY);
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
/*�����\��(�{�g�����x��)�̐���
.�X�N���b�`���\�[�X
.���U���g���\�[�X
.�t�H�[���o�b�N�p�f�o�C�X
.�t�H�[���o�b�N�p�R�}���h���X�g
.�f�B�X�N���v�^�q�[�v
.�v���~�e�B�u�p���\�[�X
.���_��*/
void CreateAcceleration(ID3D12Resource1** scratch, ID3D12Resource1** result, ID3D12RaytracingFallbackDevice* dev, ID3D12RaytracingFallbackCommandList* list, ID3D12DescriptorHeap* heap, ID3D12Resource1* primitive, const std::uint32_t& vertexNum);
/*�����\��(�g�b�v���x��)�̐���
.�X�N���b�`���\�[�X
.���U���g���\�[�X
.�C���X�^���X���\�[�X
.�t�H�[���o�b�N�p�f�o�C�X
.�t�H�[���o�b�N�p�R�}���h���X�g
.�f�B�X�N���v�^�q�[�v*/
void CreateAcceleration(ID3D12Resource1** scratch, ID3D12Resource1** result, ID3D12Resource1** instance, ID3D12RaytracingFallbackDevice* dev, ID3D12RaytracingFallbackCommandList* list, ID3D12DescriptorHeap* heap);
/*�O���[�o�����[�g�V�O�l�`���̐���
.���[�g�V�O�l�`��
.�t�H�[���o�b�N�p�f�o�C�X
.�T�u�I�u�W�F�N�g*/
void CreateGlobalRoot(ID3D12RootSignature** root, ID3D12RaytracingFallbackDevice* dev, std::vector<D3D12_STATE_SUBOBJECT>& subObj);
/*���[�J�����[�g�V�O�l�`���̐���
.���[�g�V�O�l�`��
.�t�H�[���o�b�N�p�f�o�C�X
.�T�u�I�u�W�F�N�g*/
void CreateLocalRoot(ID3D12RootSignature** root, ID3D12RaytracingFallbackDevice* dev, std::vector<D3D12_STATE_SUBOBJECT>& subObj);
/*���C�W�F�l���[�V�����p���[�J�����[�g�V�O�l�`���̐���
.���[�g�V�O�l�`��
.�t�H�[���o�b�N�p�f�o�C�X
.�T�u�I�u�W�F�N�g*/
void CreateLocalRayGenRoot(ID3D12RootSignature** root, ID3D12RaytracingFallbackDevice* dev, std::vector<D3D12_STATE_SUBOBJECT>& subObj);
/*�p�C�v���C���̐���
.�p�C�v���C��
.�t�H�[���o�b�N�p�f�o�C�X
.�T�u�I�u�W�F�N�g*/
void CreatePipe(ID3D12RaytracingFallbackStateObject** pipe, ID3D12RaytracingFallbackDevice* dev, const std::vector<D3D12_STATE_SUBOBJECT>& subObj);
/*�E�B���h�E���b�Z�[�W�̊m�F
return �I���t���O*/
bool CheckMsg(void);
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
/*�����\��(�g�b�v���x��)�̍X�V
.�����\��(�g�b�v���x��)�p�C���X�^���X���\�[�X
.�����\��(�{�g�����x��)�p���U���g���\�[�X*/
void AccelerationUpDate(ID3D12Resource1* rsc, ID3D12Resource1* bottom);
/*�V�F�[�_�̃R���p�C��
.�V�F�[�_�t�@�C���p�X
.�V�F�[�_���f��
.�V�F�[�_���
.DXIL���
.�ǂݍ��݊֐����
.�T�u�I�u�W�F�N�g
.�ǂݍ��݊֐���(�i�[�ς�)
.�ǂݍ��݊֐�����*/
void ShaderCompile(const std::wstring& filePath, const std::wstring& model, IDxcBlob** blob, 
	D3D12_DXIL_LIBRARY_DESC& dxil, std::vector<D3D12_EXPORT_DESC>& exportDesc, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const wchar_t* exportName[], const std::uint32_t& exportNameNum);
/*�q�b�g�O���[�v�̐ݒ�
.�q�b�g�O���[�v���
.�T�u�I�u�W�F�N�g
.closesthit�֐���
.�q�b�g�O���[�v��
.anyhit�֐���
.intersection�֐���*/
void SetHitGroup(D3D12_HIT_GROUP_DESC& hitDesc, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const wchar_t* closesthit, const wchar_t* hitName, const wchar_t* anyhit = nullptr, const wchar_t* intersection = nullptr);
/*�V�F�[�_�ݒ�
.�V�F�[�_�ݒ�
.�T�u�I�u�W�F�N�g
.�y�C���[�h�T�C�Y
.�A�g���r���[�g�T�C�Y*/
void SetShaderConfig(D3D12_RAYTRACING_SHADER_CONFIG& config, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const std::uint32_t& payloadSize, const std::uint32_t& attributeSize = sizeof(float) * 2);
/*�ǂݍ��݂Ƃ̊֘A�t��
.�ǂݍ��݊֘A�t�����
.�T�u�I�u�W�F�N�g
.�ǂݍ��݊֐���
.�ǂݍ��݊֐�����*/
void ExportAssociation(D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION& association, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const wchar_t* exportName[], const std::uint32_t& exportNameNum = 1);
/*�p�C�v���C���ݒ�
.�p�C�v���C���ݒ�
.�T�u�I�u�W�F�N�g
.TraceRay�Ăяo����*/
void SetPipeConfig(D3D12_RAYTRACING_PIPELINE_CONFIG& config, std::vector<D3D12_STATE_SUBOBJECT>& subObj, const std::uint32_t& depth);
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
