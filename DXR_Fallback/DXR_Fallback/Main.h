#pragma once
#include "Function/Function.h"

template <typename T>
class Obj {
public:
	/*�R���X�g���N�^*/
	Obj() : obj(nullptr) {}
	/*�f�X�g���N�^*/
	~Obj(){
		if (obj != nullptr) {
			obj->Release();
			obj = nullptr;
		}
	}
	T* Get(void) const {
		return obj;
	}
	T* operator->(void) const {
		return Get();
	}
	T** operator&(void) {
		return &obj;
	}

private:
	T* obj;
};

/*�E�B���h�E�n���h��*/
void* win = nullptr;
/*�f�o�C�X*/
Obj<ID3D12Device5>dev;
/*�t�H�[���o�b�N�p�f�o�C�X*/
Obj<ID3D12RaytracingFallbackDevice>f_dev;
/*�R�}���h�A���P�[�^*/
Obj<ID3D12CommandAllocator>allo;
/*�R�}���h�L���[*/
Obj<ID3D12CommandQueue>queue;
/*�R�}���h���X�g*/
Obj<ID3D12GraphicsCommandList4>list;
/*�t�H�[���o�b�N�p�R�}���h���X�g*/
Obj<ID3D12RaytracingFallbackCommandList>f_list;
/*�t�F���X*/
Obj<ID3D12Fence1>fence;
/*�t�F���X�J�E���g*/
std::uint64_t fenceCnt = 0;
/*�X���b�v�`�F�C��*/
Obj<IDXGISwapChain4>swap;
/*�����_�[�^�[�Q�b�g�p���\�[�X�z��*/
std::vector<Obj<ID3D12Resource1>>rtvRsc;
/*�����_�[�^�[�Q�b�g�p�q�[�v*/
Obj<ID3D12DescriptorHeap>rtvHeap;
/*�O�p�`�v���~�e�B�u�p���\�[�X*/
Obj<ID3D12Resource1>triangle;
/*�����\��(�{�g�����x��)�p�q�[�v*/
Obj<ID3D12DescriptorHeap>bottomHeap;
/*�����\��(�{�g�����x��)�p�X�N���b�`���\�[�X*/
Obj<ID3D12Resource1>bottomScratch;
/*�����\��(�{�g�����x��)�p���U���g���\�[�X*/
Obj<ID3D12Resource1>bottomResult;
/*�����\��(�g�b�v���x��)�p�q�[�v*/
Obj<ID3D12DescriptorHeap>topHeap;
/*�����\��(�g�b�v���x��)�p�X�N���b�`���\�[�X*/
Obj<ID3D12Resource1>topScratch;
/*�����\��(�g�b�v���x��)�p���U���g���\�[�X*/
Obj<ID3D12Resource1>topResult;
/*�����\��(�g�b�v���x��)�p�C���X�^���X���\�[�X*/
Obj<ID3D12Resource1>topInstance;
/*�T�u�I�u�W�F�N�g*/
std::vector<D3D12_STATE_SUBOBJECT>subObj;
/*�O���[�o�����[�g�V�O�l�`��*/
Obj<ID3D12RootSignature>global;
/*�V�F�[�_���(1�V�F�[�_��1��)*/
Obj<IDxcBlob>shader;
/*DXIL���C�u�������(1�V�F�[�_��1��)*/
D3D12_DXIL_LIBRARY_DESC dxil{};
/*�ǂݍ��݊֐����(1�V�F�[�_��1��)*/
std::vector<D3D12_EXPORT_DESC>exportDesc;
/*�ǂݍ��݊֐���(1�V�F�[�_��1��)*/
const wchar_t* exportName[3];
/*�q�b�g�O���[�v���(1�q�b�g�O���[�v��1��)*/
D3D12_HIT_GROUP_DESC hitDesc{};
/*�q�b�g�O���[�v�p�N���[�[�X�g�q�b�g�֐���*/
std::wstring closestHitName;
/*�q�b�g�O���[�v��*/
std::wstring hitName;
/*���[�J�����[�g�V�O�l�`��(1�ǂݍ��݂�1��)*/
std::vector<Obj<ID3D12RootSignature>>local;
/*�ǂݍ��݂Ƃ̊֘A�t�����(1�ǂݍ��݂�1��)*/
std::vector<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION>association;
/*�V�F�[�_�ݒ�(1�V�F�[�_��1��)*/
D3D12_RAYTRACING_SHADER_CONFIG shaderConfig{};
/*�p�C�v���C���ݒ�*/
D3D12_RAYTRACING_PIPELINE_CONFIG pipeConfig{};
/*��ԃI�u�W�F�N�g*/
Obj<ID3D12RaytracingFallbackStateObject>pipe;
/*�V�F�[�_�e�[�u���p���\�[�X(1�ǂݍ��݂�1��)*/
std::vector<Obj<ID3D12Resource1>>shaderTbl;
