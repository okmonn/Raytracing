#pragma once
#include "../SubObj.h"

class ShaderConfig :
	public SubObj
{
public:
	// コンストラクタ
	ShaderConfig(const size_t& payloadSize, const size_t& attributeSize = sizeof(float) * 2);
	// デストラクタ
	~ShaderConfig();
	
private:
	// サブオブジェクトの生成
	void CreateSub(const size_t& payloadSize, const size_t& attributeSize);
};
