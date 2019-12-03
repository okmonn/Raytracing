#pragma once
#include "../SubObj.h"

class Association :
	public SubObj
{
public:
	// コンストラクタ
	Association(const SubObj* obj, const std::initializer_list<const char*>& name);
	// デストラクタ
	~Association();

private:
	// サブオブジェクトの生成
	void CreateSub(const SubObj* obj, const wchar_t** name, const size_t& num);
};
