#include "OkdioList.h"

// コンストラクタ
OkdioList::OkdioList()
{
}

// デストラクタ
OkdioList::~OkdioList()
{
}

// リストに追加
void OkdioList::Add(Okdio** okdio)
{
	std::unique_lock<std::mutex>lock(mtx);
	this->okdio.push_back(*okdio);
}

// リストの削除
void OkdioList::Delete(Okdio* okdio)
{
	std::unique_lock<std::mutex>lock(mtx);
	for (auto itr = this->okdio.begin(); itr != this->okdio.end(); ++itr)
	{
		if ((*itr) == okdio)
		{
			this->okdio.erase(itr);
			break;
		}
	}
}

// リストの取得
std::list<Okdio*> OkdioList::GetList(void) const
{
	return okdio;
}
