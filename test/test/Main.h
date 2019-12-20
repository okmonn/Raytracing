#pragma once
#include <wrl.h>
#include <array>
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>

// �@�\���x��
const D3D_FEATURE_LEVEL levels[] =
{
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
};


namespace okmonn
{
	template <typename T>
	class  Interface
	{
	public:
		// �R���X�g���N�^
		Interface() : ptr(nullptr) {}
		// �f�X�g���N�^
		virtual ~Interface()
		{
			if (ptr != nullptr)
			{
				ptr->Release();
				ptr = nullptr;
			}
		}

		T* get(void) const
		{
			return ptr;
		}

		void operator=(const T* ptr)
		{
			this->ptr = ptr;
		}

		T** operator&(void)
		{
			return &ptr;
		}

		T* operator->(void) const
		{
			return ptr;
		}

	private:
		// �C���^�[�t�F�[�X
		T* ptr;
	};

	// �t�F���X
	class Fence
	{
	public:
		// �R���X�g���N�^
		Fence() : cnt(0) {}
		// �f�X�g���N�^
		~Fence() {}

		// �ҋ@
		void Wait(void)
		{
			if (fence->GetCompletedValue() != cnt)
			{
				void* handle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
				auto hr = fence->SetEventOnCompletion(cnt, handle);
				_ASSERT(hr == S_OK);

				WaitForSingleObject(handle, INFINITE);
			}
		}

		ID3D12Fence1* get(void) const
		{
			return fence.get();
		}

		ID3D12Fence1** operator&(void)
		{
			return &fence;
		}

		unsigned __int64 operator++(void)
		{
			cnt += 1;
			return cnt;
		}

	private:
		// �t�F���X�C���^�[�t�F�[�X
		Interface<ID3D12Fence1>fence;

		// �t�F���X�J�E���g
		unsigned __int64 cnt;
	};

	// �����\��
	class Acceleration
	{
	public:
		// �R���X�g���N�^
		Acceleration() : geo({}), input({}) {}
		// �f�X�g���N�^
		~Acceleration() {}

		// �X�N���b�`���\�[�X�̎擾
		okmonn::Interface<ID3D12Resource1> Scratch(void) const
		{
			return rsc[0];
		}

		//���U���g���\�[�X�̎擾
		okmonn::Interface<ID3D12Resource1> Scratch(void) const
		{
			return rsc[1];
		}

		//�C���X�^���X���\�[�X�̎擾
		okmonn::Interface<ID3D12Resource1> Scratch(void) const
		{
			return rsc[2];
		}

	private:
		// �W�I���g�����
		D3D12_RAYTRACING_GEOMETRY_DESC geo;

		// ���͏��
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input;

		// ���\�[�X
		std::array<okmonn::Interface<ID3D12Resource1>, 3>rsc;
	};

	// 3�����x�N�g��
	class Vec3
	{
	public:
		// �R���X�g���N�^
		Vec3()
		{
			x = y = z = 0.0f;
		}
		Vec3(const float& x, const float& y, const float& z) : 
			x(x), y(y), z(z) {
		}
		Vec3(const Vec3& vec)
		{
			(*this) = vec;
		}
		// �f�X�g���N�^
		~Vec3() {}

		void operator=(const Vec3& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
		}

	private:
		float x;
		float y;
		float z;
	};
}

// �E�B���h�E�n���h��
void* winHandle = nullptr;

// �t�@�N�g���[
okmonn::Interface<IDXGIFactory7>factory;

// �f�o�C�X
okmonn::Interface<ID3D12Device5>device;

// �R�}���h�A���P�[�^
okmonn::Interface<ID3D12CommandAllocator>allo;

// �R�}���h���X�g
okmonn::Interface<ID3D12GraphicsCommandList5>list;

// �R�}���h�L���[
okmonn::Interface<ID3D12CommandQueue>queue;

// �t�F���X
okmonn::Fence fence;

// �X���b�v�`�F�C��
okmonn::Interface<IDXGISwapChain4>swap;

// �����_�[�^�[�Q�b�g�p�q�[�v
okmonn::Interface<ID3D12DescriptorHeap>rtvHeap;

// �����_�[�^�[�Q�b�g�p���\�[�X
std::vector < okmonn::Interface<ID3D12Resource1>>rtvRsc;

// �v���~�e�B�u
okmonn::Interface<ID3D12Resource1>triangle;

// �{�g�����x��
okmonn::Acceleration bottom;