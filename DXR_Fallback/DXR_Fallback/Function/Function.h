#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3D12RaytracingFallback.h>
#include <cstdint>
#include <initializer_list>

/*ウィンドウの生成
.ウィンドウハンドル
.横幅
.縦幅*/
void CreateWnd(void** handle, const std::uint32_t& sizeX, const std::uint32_t& sizeY);
/*ウィンドウメッセージの確認
return 終了フラグ*/
bool CheckMsg(void);
/*デバイスの生成
.デバイス
.機能レベル*/
void CreateDevice(ID3D12Device5** dev, const D3D_FEATURE_LEVEL& level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1);
/*フォールバックデバイスの生成
.フォールバック用デバイス
.デバイス*/
void CreateDevice(ID3D12RaytracingFallbackDevice** f_dev, ID3D12Device5* dev);
/*コマンドアロケータの生成
.コマンドアロケータ
.デバイス
.コマンドタイプ*/
void CreateAllocator(ID3D12CommandAllocator** allo, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
/*コマンドキューの生成
.コマンドキュー
.デバイス
.コマンドタイプ
*/
void CreateQueue(ID3D12CommandQueue** queue, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
/*コマンドリストの生成
.コマンドリスト
.デバイス
.コマンドタイプ*/
void CreateList(ID3D12GraphicsCommandList4** list, ID3D12Device5* dev, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
/*フォールバック用コマンドリストの生成
.フォールバック用コマンドリスト
.フォールバック用デバイス
.コマンドリスト*/
void CreateList(ID3D12RaytracingFallbackCommandList** f_list, ID3D12RaytracingFallbackDevice* dev, ID3D12GraphicsCommandList4* list);
/*フェンスの生成
.フェンス
.デバイス*/
void CreateFence(ID3D12Fence1** fence, ID3D12Device5* dev, const std::uint64_t& initCnt = 0);
/*スワップチェインの生成
.スワップチェイン
.ウィンドウハンドル
.コマンドキュー
.バックバッファ数*/
void CreateSwapChain(IDXGISwapChain4** swap, void* win, ID3D12CommandQueue* queue, const std::uint32_t& backBuffNum = 2);
/*ディスクリプタヒープの生成
.ディスクリプタヒープ
.デバイス
.ヒープタイプ
.リソース数
.シェーダフラグ*/
void CreateHeap(ID3D12DescriptorHeap** heap, ID3D12Device5* dev, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& rscNum, const bool& shader = false);
/*リソースの生成
.リソース
.スワップチェイン
.リソースインデックス*/
void CreateRsc(ID3D12Resource1** rsc, IDXGISwapChain4* swap, const std::uint32_t& index);
/*プリミティブリソースの生成
.リソース
.デバイス
.1頂点当たりのサイズ
.頂点数*/
void CreateRsc(ID3D12Resource1** rsc, ID3D12Device5* dev, const std::uint32_t& vertexStride, const std::uint32_t& vertexNum);
/*レンダーターゲットビューの生成
.リソース
.デバイス
.ディスクリプタヒープ
.リソースインデックス*/
void RTV(ID3D12Resource1* rsc, ID3D12Device5* dev, ID3D12DescriptorHeap* heap, const std::uint32_t& index);
/*コマンドアロケータのリセット
.コマンドアロケータ*/
void Reset(ID3D12CommandAllocator* allo);
/*コマンドリストのリセット
.コマンドリスト
.コマンドアロケータ*/
void Reset(ID3D12GraphicsCommandList4* list, ID3D12CommandAllocator* allo);
/*コマンドリストのクローズ
.コマンドリスト*/
void Close(ID3D12GraphicsCommandList4* list);
/*UAVリソースバリア
.リソース
.コマンドリスト*/
void Barrier(ID3D12Resource1* rsc, ID3D12GraphicsCommandList4* list);
/*リソースバリア
.リソース
.コマンドリスト
.移行前状態
.移行後状態*/
void Barrier(ID3D12Resource1* rsc, ID3D12GraphicsCommandList4* list, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
/*バックバッファ数の取得
.スワップチェイン
return バックバッファ数*/
std::uint32_t GetBackBuffNum(IDXGISwapChain4* swap);
/*現在のバックバッファインデックスの取得
.スワップチェイン
return バックバッファインデックス*/
std::uint32_t GetBuffIndex(IDXGISwapChain4* swap);
/*データのコピー
.リソース
.データ*/
void Copy(ID3D12Resource1* rsc, void* data);
/*レンダーターゲットのクリア
.ディスクリプタヒープ
.コマンドリスト
.リソースインデックス
.クリアカラー[4]
.DSV用ディスクリプタヒープ*/
void Clear(ID3D12DescriptorHeap* heap, ID3D12Device5* dev, ID3D12GraphicsCommandList4* list, const std::uint32_t& index, const float* color, ID3D12DescriptorHeap* depth = nullptr);
/*コマンドの実行
.コマンドキュー
.コマンドリスト*/
void Execution(ID3D12CommandQueue* queue, const std::initializer_list<ID3D12GraphicsCommandList4*>& lists);
/*コマンド実行の待機
.フェンス
.コマンドキュー
.スワップチェイン
.フェンスカウント*/
void Wait(ID3D12Fence1* fence, ID3D12CommandQueue* queue, IDXGISwapChain4* swap, std::uint64_t& fenceCnt);
