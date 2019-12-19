#pragma once

namespace DXR
{
	// �R�}���h�^�C�v
	enum class CommandType
	{
		DIRECT,
		BUNDLE,
		COMPUTE,
		COPY
	};

	// ���[�g�V�O�l�`���^�C�v
	enum class RootType
	{
		GLOBAL = 1,
		LOCAL
	};

	// �V�F�[�_�^�C�v
	enum class ShaderType
	{
		RAYGEN,
		MISS,
		CLOSEST
	};
}
