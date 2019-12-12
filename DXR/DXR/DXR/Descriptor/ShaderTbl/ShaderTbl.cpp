#include "ShaderTbl.h"
#include "../Output/Output.h"
#include "../../Pipe/Pipe.h"
#include "../../Func/Func.h"
#include <wrl.h>
#include <d3d12.h>

size_t ShaderTbl::size = (((D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + 8) + (D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT - 1)) / D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT) * D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT;

// コンストラクタ
ShaderTbl::ShaderTbl(const Pipe* pipe) : 
	pipe(nullptr)
{
}

// デストラクタ
ShaderTbl::~ShaderTbl()
{
}

// リソースの生成
void ShaderTbl::CreateRsc(const size_t& num)
{
	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = size * num;

	Descriptor::CreateRsc(UploadProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// レイジェネレーション用
void ShaderTbl::RayGeneration(const std::string& name, const Output* output)
{
	CreateRsc(1);

	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>prop = nullptr;
	auto hr = pipe->Get()->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	unsigned char* buf = nullptr;
	Map((void**)&buf);

	std::memcpy(buf, prop->GetShaderIdentifier(DXR::ChangeCode(name).c_str()), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	*(unsigned long long*)(buf + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) = output->Heap()->GetGPUDescriptorHandleForHeapStart().ptr;

	UnMap();
}

// ミス用
void ShaderTbl::Miss(const std::string& name)
{
	CreateRsc(1);

	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>prop = nullptr;
	auto hr = pipe->Get()->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	unsigned char* buf = nullptr;
	Map((void**)&buf);

	std::memcpy(buf, prop->GetShaderIdentifier(DXR::ChangeCode(name).c_str()), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

	UnMap();
}

// クローゼストヒット用
void ShaderTbl::Closest(const std::string& name)
{
	CreateRsc(1);

	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>prop = nullptr;
	auto hr = pipe->Get()->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	unsigned char* buf = nullptr;
	Map((void**)&buf);

	std::memcpy(buf, prop->GetShaderIdentifier(DXR::ChangeCode(name).c_str()), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

	UnMap();
}

// シェーダエントリーサイズの取得
size_t ShaderTbl::EntrySize(void)
{
	return size;
}
