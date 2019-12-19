#pragma once
#include "../Info.h"

enum D3D12_RESOURCE_STATES : int;
struct ID3D12GraphicsCommandList5;
struct ID3D12Resource;
class Window;
class Allocator;
class Pipe;
class Root;

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

	// ビューポートのセット
	void Viewport(const Window* win);

	// シザーのセット
	void Scissor(const Window* win);

	// パイプラインのセット
	void SetPipe(const Pipe* pipe);

	// ルートシグネチャのセット
	void SetRoot(const Root* root);

	// コマンドリストの取得
	ID3D12GraphicsCommandList5* Get(void) const;

private:
	// コマンドリストの生成
	void CreateList(const DXR::CommandType& type);


	// コマンドリスト
	ID3D12GraphicsCommandList5* list;
};
