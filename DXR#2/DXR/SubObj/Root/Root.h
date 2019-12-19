#pragma once
#include "../SubObj.h"
#include "../../Info.h"
#include <vector>

struct D3D12_DESCRIPTOR_RANGE;
struct D3D12_ROOT_PARAMETER;
struct D3D12_ROOT_SIGNATURE_DESC;
struct ID3D12RootSignature;
class ShaderLibrary;

namespace DXR
{
	// ルートシグネチャ情報
	struct RootInfo
	{
		std::vector<D3D12_DESCRIPTOR_RANGE>range;
		std::vector<D3D12_ROOT_PARAMETER>param;
		std::unique_ptr<D3D12_ROOT_SIGNATURE_DESC>desc;

		RootInfo() {
			desc = std::make_unique<D3D12_ROOT_SIGNATURE_DESC>();
		}
		RootInfo(const size_t& rangeNum, const size_t& paramNum) {
			range.resize(rangeNum);
			param.resize(paramNum);
			desc = std::make_unique<D3D12_ROOT_SIGNATURE_DESC>();
		}
	};
}

class Root :
	public SubObj
{
public:
	// コンストラクタ
	Root(const DXR::RootType& type, const DXR::RootInfo& info);
	Root(const ShaderLibrary* shaderLib);
	// デストラクタ
	~Root();

	// レイジェネレーション用ルートシグネチャ情報
	static DXR::RootInfo RayGenDesc(void);

	// ルートシグネチャの取得
	ID3D12RootSignature* RtSig(void) const;

private:
	// ルートシグネチャの生成
	void CreateRoot(const DXR::RootInfo& info);
	void CreateRoot(const ShaderLibrary* shaderLib);

	// サブオブジェクトの設定
	void SetSubObj(const DXR::RootType& type);


	// ルートシグネチャ
	ID3D12RootSignature* root;
};
