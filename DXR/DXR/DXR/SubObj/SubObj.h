#pragma once
#include <string>
#include <vector>
#include <memory>

struct D3D12_STATE_SUBOBJECT;
struct ID3D10Blob;
using ID3DBlob = ID3D10Blob;

class SubObj
{
public:
	// �R���X�g���N�^
	SubObj(const std::string& fileName, const std::string& ver, const std::initializer_list<std::string>& func, const std::string& entry = "");
	// �f�X�g���N�^
	~SubObj();

	// �T�u�I�u�W�F�N�g�̎擾
	D3D12_STATE_SUBOBJECT Get(void) const;

	// �V�F�[�_�u���u�̎擾
	ID3DBlob* Blob(void) const;

private:
	// �T�u�I�u�W�F�N�g�̐ݒ�
	void SetSubObj(const std::initializer_list<std::string>& func);


	// �T�u�I�u�W�F�N�g
	std::unique_ptr<D3D12_STATE_SUBOBJECT>sub;

	// �V�F�[�_�u���u
	ID3DBlob* blob;
};
