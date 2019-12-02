#pragma once
#include "../SubObj.h"
#include <string>

struct D3D12_HIT_GROUP_DESC;

class Hit :
	public SubObj
{
public:
	// コンストラクタ
	Hit(const std::string& hitName, const char* closest, const char* any = nullptr);
	// デストラクタ
	~Hit();

private:
	// サブオブジェクトの生成
	void CreateSub(const wchar_t* closest, const wchar_t* any);


	// ヒットグループ情報
	std::unique_ptr<D3D12_HIT_GROUP_DESC>group;

	// ヒットグループ名
	std::wstring hitName;
};
