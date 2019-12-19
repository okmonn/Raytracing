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

	// �V�F�[�_�^�C�v
	enum class ShaderType
	{
		RAYGEN,
		MISS,
		CLOSEST
	};
}
