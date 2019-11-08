#pragma once
#include "ShaderLibrary.h"
#include <wrl.h>
#include <array>
#include <dxgi1_6.h>

// �������J��
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// �E�B���h�E�T�C�YX
#define WINSIZE_X 640
// �E�B���h�E�T�C�YY
#define WINSIZE_Y 480

// �o�b�N�o�b�t�@�[�J�E���g
#define BACK_BUFFER 3

// �@�\���x���ꗗ
const D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
};

// �t�F���X
struct Fence
{
	//�t�F���X
	ID3D12Fence* ptr;
	//�J�E���g
	unsigned __int32 cnt;
	//�C�x���g
	void* event;

	Fence() {
		ptr   = nullptr;
		cnt   = 0;
		event = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	}
};

// �����_�[�^�[�Q�b�g
struct Render
{
	//�q�[�v
	ID3D12DescriptorHeap* heap;
	//���\�[�X
	std::vector<ID3D12Resource*>rsc;

	Render() {
		heap = nullptr;
	}
};

// �����\��
struct Acceleration
{
	ID3D12Resource* scratch;
	ID3D12Resource* result;
	ID3D12Resource* instance;
	D3D12_RAYTRACING_GEOMETRY_DESC geoDesc;
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS input;

	Acceleration() {
		scratch = result = instance = nullptr;
		geoDesc = {};
		input = {};
	}
};

// ���[�g�V�O�l�`��
struct RootSignature
{
	ID3D12RootSignature* root;
	std::vector<D3D12_DESCRIPTOR_RANGE>range;
	std::vector<D3D12_ROOT_PARAMETER>param;

	RootSignature() {
		root = nullptr;
		range = {};
		param = {};
	}
};

// ��������
struct Hit
{
	std::wstring name;
	D3D12_HIT_GROUP_DESC desc;
	D3D12_STATE_SUBOBJECT sub;

	Hit(LPCWSTR anyHit, LPCWSTR closestHit, const std::wstring& name)
		: name(name)
	{
		desc.AnyHitShaderImport       = anyHit;
		desc.ClosestHitShaderImport   = closestHit;
		desc.HitGroupExport           = name.c_str();
		desc.IntersectionShaderImport = nullptr;
		desc.Type                     = D3D12_HIT_GROUP_TYPE::D3D12_HIT_GROUP_TYPE_TRIANGLES;

		sub.pDesc = &desc;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
	}
};

// 
struct Association
{
	D3D12_STATE_SUBOBJECT sub;
	D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION association;

	Association(LPCWSTR name, const unsigned int& cnt, const D3D12_STATE_SUBOBJECT* subObject) {
		association.NumExports            = cnt;
		association.pExports              = &name;
		association.pSubobjectToAssociate = subObject;

		sub.pDesc = &association;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_SUBOBJECT_TO_EXPORTS_ASSOCIATION;
	}
};

void CreateRoot(ID3D12Device5* device, RootSignature& rootsignature, const D3D12_ROOT_SIGNATURE_DESC& desc);

// ���[�J�����[�g�V�O�l�`��
struct LocalRoot
{
	RootSignature rootsignature;
	D3D12_STATE_SUBOBJECT sub;

	LocalRoot() {
		sub.pDesc = &rootsignature.root;
		sub.Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
	}
	LocalRoot(ID3D12Device5* device, const D3D12_ROOT_SIGNATURE_DESC& desc) {
		CreateRoot(device, rootsignature, desc);

		sub.pDesc = &rootsignature.root;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
	}
	~LocalRoot() {
		Release(rootsignature.root);
	}
};

// �O���[�o�����[�g�V�O�l�`��
struct GlobalRoot
{
	RootSignature rootsignature;
	D3D12_STATE_SUBOBJECT sub;

	GlobalRoot() {
		sub.Type = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
	}
	GlobalRoot(ID3D12Device5* device, const D3D12_ROOT_SIGNATURE_DESC& desc) {
		CreateRoot(device, rootsignature, desc);

		sub.pDesc = &rootsignature.root;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
	}
	~GlobalRoot() {
		Release(rootsignature.root);
	}
};

// �V�F�[�_�\��
struct ShaderConfig
{
	D3D12_RAYTRACING_SHADER_CONFIG config;
	D3D12_STATE_SUBOBJECT sub;

	ShaderConfig(const unsigned int& attributeSize, const unsigned int& payloadSize) {
		config.MaxAttributeSizeInBytes = attributeSize;
		config.MaxPayloadSizeInBytes   = payloadSize;

		sub.pDesc = &config;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
	}
};

// �p�C�v���C���\��
struct PipeConfig
{
	D3D12_RAYTRACING_PIPELINE_CONFIG config;
	D3D12_STATE_SUBOBJECT sub;

	PipeConfig(const unsigned int& traceDepth) {
		config.MaxTraceRecursionDepth = traceDepth;

		sub.pDesc = &config;
		sub.Type  = D3D12_STATE_SUBOBJECT_TYPE::D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
	}
};

// 3����
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() {
		x = y = z = 0.0f;
	}
	Vector3(const float& x, const float& y, const float& z) : 
		x(x), y(y), z(z){}
	Vector3(const Vector3& vec) {
		(*this) = vec;
	}
	void operator=(const Vector3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
};

// �N���A�J���[
float color[] = {
	1.0f, 1.0f, 1.0f, 1.0f
};
