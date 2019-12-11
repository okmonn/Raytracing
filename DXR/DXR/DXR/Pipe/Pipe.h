#pragma once
#include <initializer_list>

struct ID3D12StateObject;
class SubObj;

class Pipe
{
public:
	// コンストラクタ
	Pipe(const std::initializer_list<SubObj*>& obj);
	// デストラクタ
	~Pipe();

	// パイプラインの取得
	ID3D12StateObject* Get(void) const;

private:
	// パイプラインの生成
	void CreatePipe(const std::initializer_list<SubObj*>& obj);


	// パイプライン
	ID3D12StateObject* pipe;
};
