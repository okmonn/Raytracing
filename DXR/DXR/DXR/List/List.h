#pragma once
#include "../Information.h"

struct ID3D12GraphicsCommandList5;

class List
{
public:
	// コンストラクタ
	List(const DXR::CommandType& type);
	// デストラク
	~List();

	// コマンドリストの取得
	ID3D12GraphicsCommandList5* Get(void) const;
	
private:
	// コマンドリストの生成
	void CreateList(const DXR::CommandType& type);


	// コマンドリスト
	ID3D12GraphicsCommandList5* list;
};
