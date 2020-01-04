#pragma once
#include "../Singleton.h"
#include <list>
#include <mutex>

class Okdio;

class OkdioList :
	public Singleton<OkdioList>
{
	friend Singleton<OkdioList>;
public:
	// リストに追加
	void Add(Okdio** okdio);

	// リストの削除
	void Delete(Okdio* okdio);

	// リストの取得
	std::list<Okdio*>GetList(void) const;

private:
	// コンストラクタ
	OkdioList();
	// デストラクタ
	~OkdioList();


	// 排他制御
	std::mutex mtx;

	// リスト
	std::list<Okdio*>okdio;
};
