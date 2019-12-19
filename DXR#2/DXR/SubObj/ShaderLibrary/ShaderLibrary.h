#pragma once
#include "../SubObj.h"
#include <string>
#include <vector>

struct D3D12_EXPORT_DESC;
struct D3D12_DXIL_LIBRARY_DESC;
struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

class ShaderLibrary :
	public SubObj
{
public:
	// �R���X�g���N�^
	ShaderLibrary(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry = "");
	// �f�X�g���N�^
	~ShaderLibrary();

	// �V�F�[�_�u���u�̎擾
	ID3DBlob* Blob(void) const;

private:
	// �T�u�I�u�W�F�N�g�̐ݒ�
	void SetSubObj(const std::initializer_list<std::string>& func);


	// �V�F�[�_�u���u
	ID3DBlob* blob;

	// DXIL���C�u����
	std::unique_ptr<D3D12_DXIL_LIBRARY_DESC>lib;

	// �G�N�X�|�[�g��
	std::vector<std::wstring>name;

	// �G�N�X�|�[�g���
	std::vector<D3D12_EXPORT_DESC>expo;
};
