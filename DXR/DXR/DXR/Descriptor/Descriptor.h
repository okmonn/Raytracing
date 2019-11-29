#pragma once
#include <vector>

enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
enum D3D12_HEAP_FLAGS : int;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct D3D12_HEAP_PROPERTIES;
struct D3D12_RESOURCE_DESC;
struct D3D12_CLEAR_VALUE;

class Descriptor
{
public:
	// ヒープの取得
	ID3D12DescriptorHeap* Heap(void) const;

	// リソースの取得
	ID3D12Resource* Rsc(const size_t& index) const;

	// リソースの取得
	std::vector<ID3D12Resource*>Rsc(void) const;
		
private:

protected:
	// コンストラクタ
	Descriptor();
	// デストラク
	virtual ~Descriptor();

	// ヒープの生成
	void CreateHeap(const D3D12_DESCRIPTOR_HEAP_TYPE& type, const size_t& rscNum, const bool& shader = false);

	// デフォルトプロパティの取得
	D3D12_HEAP_PROPERTIES DefaultProp(void);

	// アップロードプロパティの取得
	D3D12_HEAP_PROPERTIES UploadProp(void);

	// リソースの生成
	void CreateRsc(const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc, const D3D12_RESOURCE_STATES& state, 
		const size_t& index = 0, const D3D12_HEAP_FLAGS& flag = D3D12_HEAP_FLAGS(0), const D3D12_CLEAR_VALUE* clear = nullptr);

	// マップ
	void Map(void** buf, const size_t& index = 0);

	// アンマップ
	void UnMap(const size_t& index = 0);


	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース
	std::vector<ID3D12Resource*>rsc;
};
