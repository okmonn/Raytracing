#pragma once
#include "../Information.h"

enum D3D12_RESOURCE_STATES : int;
struct ID3D12GraphicsCommandList5;
struct ID3D12Resource;
class Allocator;
class Root;
class Pipe;
class Descriptor;

class List
{
public:
	// コンストラクタ
	List(const DXR::CommandType& type);
	// デストラク
	~List();

	// コマンドリストのリセット
	void Reset(const Allocator* allo) const;

	// コマンドリストのクローズ
	void Close(void) const;

	// バリア
	void Barrier(ID3D12Resource* rsc, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) const;

	// UAVバリア
	void Barrier(ID3D12Resource* rsc) const;

	// ルートシグネチャのセット
	void SetRoot(const Root* root);

	// パイプラインのセット
	void SetPipe(const Pipe* pipe);

	// ディスクリプターヒープのセット
	void SetHeap(const Descriptor* descriptor, const size_t& num);

	// コマンドリストの取得
	ID3D12GraphicsCommandList5* Get(void) const;
	
private:
	// コマンドリストの生成
	void CreateList(const DXR::CommandType& type);


	// コマンドリスト
	ID3D12GraphicsCommandList5* list;
};
