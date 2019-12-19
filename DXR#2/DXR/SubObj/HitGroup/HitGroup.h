#pragma once
#include "../SubObj.h"
#include <string>

struct D3D12_HIT_GROUP_DESC;

class HitGroup :
	public SubObj
{
public:
	// コンストラクタ
	HitGroup(const std::string& hitName, const std::string& closestHit);
	// デストラクタ
	~HitGroup();
	
private:
	// サブオブジェクトの設定
	void SetSubObj(void);


	// ヒットグループ名
	std::wstring hitName;

	// クローゼスト名
	std::wstring closest;

	// ヒットグループ情報
	std::unique_ptr<D3D12_HIT_GROUP_DESC>desc;
};
