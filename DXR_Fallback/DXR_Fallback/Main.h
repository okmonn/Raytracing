#pragma once
#include "Function/Function.h"
#include <vector>

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
