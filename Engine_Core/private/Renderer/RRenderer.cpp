#include "Renderer/RRenderer.h"
#include "Graphics/RGraphics.h"
#include "Resource/RResources.h"
#include "Resource/RShader.h"
#include "Resource/RMesh.h"
#include "Resource/RMaterial.h"
#include "Resource/RTexture.h"
#include "Component/Camera/JCamera.h"
#include "Graphics/GPUBuffer/RConstantBuffer.h"
#include "HighLevelInterface/IApplication.h"
#include "Graphics/RRenderTarget.h"

extern IApplication application;

namespace renderer
{
	JCamera* mainCamera = nullptr;
	AActor* selectedActor = nullptr;
	RConstantBuffer* constantBuffers[(UINT)ECBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)ESamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)ERasterizerState::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)EBlendState::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)EDepthStencilState::End] = {};
	RRenderTarget* FrameBuffer = nullptr;

	void LoadStates()
	{
#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)ESamplerType::Anisotropic].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)ESamplerType::Point].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)ESamplerType::Linear].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)ESamplerType::PostProcess].GetAddressOf());

		GetDevice()->BindSamplers((UINT)ESamplerType::Point, 1,
			samplerStates[(UINT)ESamplerType::Point].GetAddressOf());
		GetDevice()->BindSamplers((UINT)ESamplerType::Linear, 1,
			samplerStates[(UINT)ESamplerType::Linear].GetAddressOf());
		GetDevice()->BindSamplers((UINT)ESamplerType::Anisotropic, 1,
			samplerStates[(UINT)ESamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSamplers((UINT)ESamplerType::PostProcess, 1,
			samplerStates[(UINT)ESamplerType::PostProcess].GetAddressOf());
#pragma endregion

#pragma region rasterize state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.AntialiasedLineEnable = false;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rsDesc.DepthBias = 0;
		rsDesc.DepthBiasClamp = 0.0f;
		rsDesc.DepthClipEnable = true;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.MultisampleEnable = false;
		rsDesc.ScissorEnable = false;
		rsDesc.SlopeScaledDepthBias = 0.0f;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)ERasterizerState::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)ERasterizerState::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)ERasterizerState::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)ERasterizerState::Wireframe].GetAddressOf());
#pragma endregion
#pragma region blend state
		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)EBlendState::AlphaBlend].GetAddressOf());

		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)EBlendState::OneOne].GetAddressOf());
#pragma endregion
#pragma region depthstencil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)EDepthStencilState::LessEqual].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)EDepthStencilState::DepthNone].GetAddressOf());
#pragma endregion
	}

	void LoadTriangleMesh()
	{
		RMesh* mesh = new RMesh();

		vector<FVertex> vertexes = {};
		vector<UINT> indices = {};

		vertexes.resize(3);
		vertexes[0].pos = FVector3(0.0f, 0.5f, 0.0f);
		vertexes[0].color = FVector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[1].pos = FVector3(0.5f, -0.5f, 0.0f);
		vertexes[1].color = FVector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[2].pos = FVector3(-0.5f, -0.5f, 0.0f);
		vertexes[2].color = FVector4(0.0f, 0.0f, 1.0f, 1.0f);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[2] = {};
		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		RShader* triangleShader = RResources::Find<RShader>(L"TriangleShader");
		mesh->SetVertexBufferParams(2, inputLayoutDesces, triangleShader->GetVSBlob()->GetBufferPointer(), triangleShader->GetVSBlob()->GetBufferSize());

		mesh->CreateVB(vertexes);
		mesh->CreateIB(indices);

		RResources::Insert(L"TriangleMesh", mesh);
	}

	void LoadRectMesh()
	{
		RMesh* mesh = new RMesh();

		vector<FVertex> vertexes = {};
		vector<UINT> indices = {};

		vertexes.resize(4);
		vertexes[0].pos = FVector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = FVector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[0].uv = FVector2(0.0f, 0.0f);

		vertexes[1].pos = FVector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = FVector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[1].uv = FVector2(1.0f, 0.0f);

		vertexes[2].pos = FVector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = FVector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = FVector2(1.0f, 1.0f);

		vertexes[3].pos = FVector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = FVector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[3].uv = FVector2(0.0f, 1.0f);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[3] = {};
		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		inputLayoutDesces[2].AlignedByteOffset = 28;
		inputLayoutDesces[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputLayoutDesces[2].InputSlot = 0;
		inputLayoutDesces[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[2].SemanticName = "TEXCOORD";
		inputLayoutDesces[2].SemanticIndex = 0;

		RShader* spriteShader = RResources::Find<RShader>(L"SpriteDefaultShader");
		mesh->SetVertexBufferParams(
			3,
			inputLayoutDesces,
			spriteShader->GetVSBlob()->GetBufferPointer(),
			spriteShader->GetVSBlob()->GetBufferSize());

		mesh->CreateVB(vertexes);
		mesh->CreateIB(indices);

		RResources::Insert(L"RectMesh", mesh);
	}

	void LoadMeshes()
	{
		LoadTriangleMesh();
		LoadRectMesh();
	}

	void LoadShaders()
	{
		map<EShaderStage, wstring> triangleShaderPaths = 
		{
			{ EShaderStage::VS, L"../JaehuruEngine_Core/Engine_Core/Shaders/VS/TriangleVS.hlsl" },
			{ EShaderStage::PS, L"../JaehuruEngine_Core/Engine_Core/Shaders/PS/TrianglePS.hlsl" },
		};

		map<EShaderStage, wstring> spriteShaderPaths = 
		{
			{ EShaderStage::VS, L"../JaehuruEngine_Core/Engine_Core/Shaders/VS/SpriteDefaultVS.hlsl" },
			{ EShaderStage::PS, L"../JaehuruEngine_Core/Engine_Core/Shaders/PS/SpriteDefaultPS.hlsl" },
		};

		map<EShaderStage, wstring> wireframeShaderPaths =
		{
			{ EShaderStage::VS, L"../JaehuruEngine_Core/Engine_Core/Shaders/VS/WireframeVS.hlsl" },
			{ EShaderStage::PS, L"../JaehuruEngine_Core/Engine_Core/Shaders/PS/WireframePS.hlsl" },
		};

		RResources::Load<RShader>(L"TriangleShader", triangleShaderPaths);
		RResources::Load<RShader>(L"SpriteDefaultShader", spriteShaderPaths);
		RResources::Load<RShader>(L"WireframeShader", wireframeShaderPaths);
	}

	void LoadMeterails()
	{
		RMaterial* triangleMaterial = new RMaterial();
		triangleMaterial->SetShader(RResources::Find<RShader>(L"TriangleShader"));
		RResources::Insert(L"TriangleMaterial", triangleMaterial);

		RMaterial* spriteMaterial = new RMaterial();
		spriteMaterial->SetShader(RResources::Find<RShader>(L"SpriteDefaultShader"));

		RResources::Insert(L"SpriteDefaultMaterial", spriteMaterial);
	}

	void LoadConstantBuffers()
	{
		constantBuffers[CBSLOT_TRANSFORM] = new RConstantBuffer(ECBType::JTransform);
		constantBuffers[CBSLOT_TRANSFORM]->Create(sizeof(JTransformCB));
	}

	void LoadFrameBuffer()
	{
		FRenderTargetSpecification spec;
		spec.Attachments = { ERenderTragetFormat::RGBA8, ERenderTragetFormat::Depth };
		spec.Width = application.GetWidth();
		spec.Height = application.GetHeight();

		FrameBuffer = RRenderTarget::Create(spec);
	}

	void Initialize()
	{
		LoadStates();
		LoadShaders();
		LoadMeshes();
		LoadMeterails();
		LoadConstantBuffers();
		LoadFrameBuffer();
	}

	void Release()
	{
		for (UINT i = 0; i < (UINT)ECBType::End; i++)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
	}
}
