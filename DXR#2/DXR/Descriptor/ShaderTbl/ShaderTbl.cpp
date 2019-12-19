#include "ShaderTbl.h"
#include "../../Pipe/Pipe.h"
#include "../Output/Output.h"
#include "../../Func/Func.h"
#include <wrl.h>
#include <d3d12.h>

// コンストラクタ
ShaderTbl::ShaderTbl(const Pipe* pipe) : 
	pipe(pipe)
{
}

// デストラクタ
ShaderTbl::~ShaderTbl()
{
}

// リソースの生成
void ShaderTbl::CreateRsc(const size_t& entryNum)
{
	D3D12_RESOURCE_DESC desc{};
	desc.DepthOrArraySize = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels = 1;
	desc.SampleDesc = { 1, 0 };
	desc.Width = EntrySize() * entryNum;

	Descriptor::CreateRsc(UploadProp(), desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

// レイジェネレーション
void ShaderTbl::RayGeneration(const Output* output, const std::initializer_list<std::string>& name)
{
	/*CreateRsc(name.size());

	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>prop = nullptr;
	auto hr = pipe->Get()->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	unsigned char* buf = nullptr;
	Map((void**)&buf);
	for (auto& i : name)
	{
		std::memcpy(buf, prop->GetShaderIdentifier(DXR::ChangeCode(i).c_str()), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
		*(unsigned int*)(buf + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) = output->Heap()->GetGPUDescriptorHandleForHeapStart().ptr;
	}
	UnMap();*/

	CreateRsc(3);
	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>prop = nullptr;
	auto hr = pipe->Get()->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	unsigned char* buf = nullptr;
	Map((void**)&buf);
	std::memcpy(buf, prop->GetShaderIdentifier(L"RayGen"), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	*(unsigned int*)(buf + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES) = output->Heap()->GetGPUDescriptorHandleForHeapStart().ptr;

	std::memcpy(buf + ShaderTbl::EntrySize(), prop->GetShaderIdentifier(L"Miss"), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);

	std::memcpy(buf + ShaderTbl::EntrySize() * 2, prop->GetShaderIdentifier(L"hit"), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
}

// ミス
void ShaderTbl::Miss(const std::initializer_list<std::string>& name)
{
	CreateRsc(name.size());

	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>prop = nullptr;
	auto hr = pipe->Get()->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	unsigned char* buf = nullptr;
	Map((void**)&buf);
	for (auto& i : name)
	{
		std::memcpy(buf, prop->GetShaderIdentifier(DXR::ChangeCode(i).c_str()), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	}
	UnMap();
}

// クローゼストヒット
void ShaderTbl::ClosestHit(const std::initializer_list<std::string>& name)
{
	CreateRsc(name.size());

	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties>prop = nullptr;
	auto hr = pipe->Get()->QueryInterface(IID_PPV_ARGS(&prop));
	_ASSERT(hr == S_OK);

	unsigned char* buf = nullptr;
	Map((void**)&buf);
	for (auto& i : name)
	{
		std::memcpy(buf, prop->GetShaderIdentifier(DXR::ChangeCode(i).c_str()), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	}
	UnMap();
}

// エントリーサイズの取得
size_t ShaderTbl::EntrySize(void)
{
	return (((D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES + 8) + (D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT - 1)) / D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT) * D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT;
}
