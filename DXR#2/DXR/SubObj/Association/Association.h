#pragma once
#include "../SubObj.h"
#include <string>
#include <vector>

struct D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION;

class Association :
	public SubObj
{
public:
	// コンストラクタ
	Association(const SubObj* obj, const std::initializer_list<const wchar_t*>& exportName);
	// デストラクタ
	~Association();

private:
	// サブオブジェクトの設定
	void SetSubObj(const SubObj* obj, const std::initializer_list<const wchar_t*>& exportName);


	// 関連付け
	std::unique_ptr<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION>asso;

	// エクスポート名
	std::vector<const wchar_t*>name;
};
