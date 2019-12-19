#pragma once

namespace DXR
{
	// コマンドタイプ
	enum class CommandType
	{
		DIRECT,
		BUNDLE,
		COMPUTE,
		COPY
	};

	// シェーダタイプ
	enum class ShaderType
	{
		RAYGEN,
		MISS,
		CLOSEST
	};
}
