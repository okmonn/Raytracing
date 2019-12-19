#pragma once
#include "../SubObj.h"
#include "../../Info.h"
#include <vector>

struct D3D12_DESCRIPTOR_RANGE;
struct D3D12_ROOT_PARAMETER;
struct D3D12_ROOT_SIGNATURE_DESC;
struct ID3D12RootSignature;
class ShaderLibrary;

namespace DXR
{
	// ���[�g�V�O�l�`�����
	struct RootInfo
	{
		std::vector<D3D12_DESCRIPTOR_RANGE>range;
		std::vector<D3D12_ROOT_PARAMETER>param;
		std::unique_ptr<D3D12_ROOT_SIGNATURE_DESC>desc;

		RootInfo() {
			desc = std::make_unique<D3D12_ROOT_SIGNATURE_DESC>();
		}
		RootInfo(const size_t& rangeNum, const size_t& paramNum) {
			range.resize(rangeNum);
			param.resize(paramNum);
			desc = std::make_unique<D3D12_ROOT_SIGNATURE_DESC>();
		}
	};
}

class Root :
	public SubObj
{
public:
	// �R���X�g���N�^
	Root(const DXR::RootType& type, const DXR::RootInfo& info);
	Root(const ShaderLibrary* shaderLib);
	// �f�X�g���N�^
	~Root();

	// ���C�W�F�l���[�V�����p���[�g�V�O�l�`�����
	static DXR::RootInfo RayGenDesc(void);

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* RtSig(void) const;

private:
	// ���[�g�V�O�l�`���̐���
	void CreateRoot(const DXR::RootInfo& info);
	void CreateRoot(const ShaderLibrary* shaderLib);

	// �T�u�I�u�W�F�N�g�̐ݒ�
	void SetSubObj(const DXR::RootType& type);


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;
};
