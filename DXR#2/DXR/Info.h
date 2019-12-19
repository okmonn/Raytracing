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

	// ルートシグネチャタイプ
	enum class RootType
	{
		GLOBAL = 1,
		LOCAL
	};

	// シェーダタイプ
	enum class ShaderType
	{
		RAYGEN,
		MISS,
		CLOSEST
	};
}
