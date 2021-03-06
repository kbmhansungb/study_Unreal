#include "MyTestFunctionLibrary.h"
#include "MyTest.h"

#include "TextureResource.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "GlobalShader.h"

struct FMyBufferStruct
{
	FVector4f Position;
	FVector2f UV;
};

class FMyVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;

	virtual ~FMyVertexDeclaration() {}

	virtual void InitRHI() override
	{
		FVertexDeclarationElementList Elements;
		const uint16 Stride = sizeof(FMyBufferStruct);
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FMyBufferStruct, Position), VET_Float4, 0, Stride));
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FMyBufferStruct, UV), VET_Float2, 1, Stride));
		//VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
		VertexDeclarationRHI = PipelineStateCache::GetOrCreateVertexDeclaration(Elements);
	}

	virtual void ReleaseRHI() override
	{
		VertexDeclarationRHI.SafeRelease();
	}
};

class FMyVertexBuffer : public FVertexBuffer
{
public:
	static const uint32 VerticiesNum = 4;
	static const uint32 TriangleNum = 2;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(L"MyGlobalShader");
		VertexBufferRHI = RHICreateVertexBuffer(sizeof(FMyBufferStruct) * VerticiesNum, BUF_Static, CreateInfo);
		void* VoidPtr = RHILockBuffer(VertexBufferRHI, 0, sizeof(FMyBufferStruct) * VerticiesNum, RLM_WriteOnly);

		FMyBufferStruct* Vertices = reinterpret_cast<FMyBufferStruct*>(VoidPtr);
		Vertices[0].Position = FVector4f(-1.0f, 1.0f, 0.0f, 1.0f);
		Vertices[1].Position = FVector4f(1.0f, 1.0f, 0.0f, 1.0f);
		Vertices[2].Position = FVector4f(-1.0f, -1.0f, 0.0f, 1.0f);
		Vertices[3].Position = FVector4f(1.0f, -1.0f, 0.0f, 1.0f);
		Vertices[0].UV = FVector2f(0.0f, 0.0f);
		Vertices[1].UV = FVector2f(1.0f, 0.0f);
		Vertices[2].UV = FVector2f(0.0f, 1.0f);
		Vertices[3].UV = FVector2f(1.0f, 1.0f);

		RHIUnlockBuffer(VertexBufferRHI);
	}
};

class FMyIndexBuffer : public FIndexBuffer
{
public:
	using TYPE = uint16;
	static const uint32 Stride = sizeof(TYPE);
	static const uint32 Size = Stride * 6;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(L"MyGlobalShader");
		IndexBufferRHI = RHICreateIndexBuffer(Stride, Size, BUF_Static, CreateInfo);
		void* VoidPtr = RHILockBuffer(IndexBufferRHI, 0, Size, RLM_WriteOnly);

		TYPE* Indices = reinterpret_cast<TYPE*>(VoidPtr);
		Indices[0] = 0;
		Indices[1] = 1;
		Indices[2] = 2;
		Indices[3] = 2;
		Indices[4] = 1;
		Indices[5] = 3;

		RHIUnlockBuffer(IndexBufferRHI);
	}
};

TGlobalResource<FMyVertexDeclaration> MyVertexDeclaration;
TGlobalResource<FMyVertexBuffer> VertexBuffer;
TGlobalResource<FMyIndexBuffer> IndexBuffer;


static void UseGlobalShader_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	ERHIFeatureLevel::Type FeatureLevel,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	FRHITexture2D* SrcTexture
)
{
	check(IsInRenderingThread());

	FRHIRenderPassInfo RPInfo(
		OutputRenderTargetResource->GetRenderTargetTexture(),
		ERenderTargetActions::DontLoad_Store
	);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("MyGlobalShader"));

	auto GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
	TShaderMapRef<FMyTestVS> VertexShader(GlobalShaderMap);
	TShaderMapRef<FMyTestPS> PixelShader(GlobalShaderMap);

	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

	GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
	GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
	GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = MyVertexDeclaration.VertexDeclarationRHI;
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
	GraphicsPSOInit.PrimitiveType = PT_TriangleList;

	SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, EApplyRendertargetOption::CheckApply);

	FMyTestPS::FParameters PSParameter;
	PSParameter.SrcTexture = SrcTexture;
	PSParameter.SrcTextureSampler = TStaticSamplerState<>::GetRHI();
	SetShaderParameters(RHICmdList, PixelShader, PixelShader.GetPixelShader(), PSParameter);

	RHICmdList.SetStreamSource(0, VertexBuffer.VertexBufferRHI, 0);
	RHICmdList.DrawIndexedPrimitive(IndexBuffer.IndexBufferRHI, 0, 0, VertexBuffer.VerticiesNum, 0, VertexBuffer.TriangleNum, 1);

	RHICmdList.EndRenderPass();
}

FORCEINLINE static void UseGlobalShader_RenderThread (
	FRHICommandListImmediate& RHICmdList,
	ERHIFeatureLevel::Type FeatureLevel,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	FTextureResource* SrcTexture
)
{
	UseGlobalShader_RenderThread(RHICmdList, FeatureLevel, OutputRenderTargetResource, SrcTexture->GetTexture2DRHI());
}

void UGlobalTestShaderBlueprintLibrary::UseGlobalShader(
	UTextureRenderTarget2D* OutputRenderTarget,
	UTexture* SrcTexture,
	AActor* Actor
)
{
	check(IsInGameThread());

	if (!OutputRenderTarget)
	{
		return;
	}
	if (!Actor)
	{
		return;
	}
	if (!SrcTexture)
	{
		return;
	}
	UWorld* World = Actor->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();

	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
	FTextureResource* TextureResource = SrcTexture->GetResource();

	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
			[FeatureLevel, TextureRenderTargetResource, TextureResource]
			(FRHICommandListImmediate& RHICmdList)
			{
				UseGlobalShader_RenderThread(RHICmdList, FeatureLevel, TextureRenderTargetResource, TextureResource);
			}
		);
}

static void UseComputeShader_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	ERHIFeatureLevel::Type FeatureLevel
)
{
	check(IsInRenderingThread());

	TShaderMapRef<FMyTestCS> ComputeShader(GetGlobalShaderMap(FeatureLevel));
	
	const uint32 SizeX = OutputRenderTargetResource->GetSizeX();
	const uint32 SizeY = OutputRenderTargetResource->GetSizeY();

	RHICmdList.SetComputeShader(ComputeShader.GetComputeShader());

	FRHIResourceCreateInfo CreateInfo(L"TempTexture");
	FTexture2DRHIRef Texture
		= RHICreateTexture2D(
			SizeX, SizeY, PF_A32B32G32R32F, 1, 1,
			TexCreate_ShaderResource | TexCreate_UAV, CreateInfo
		);
	FUnorderedAccessViewRHIRef TextureUAV = RHICreateUnorderedAccessView(Texture);
	
	FMyTestCS::FParameters Parameter;
	Parameter.OutputTexture = TextureUAV;
	SetShaderParameters(RHICmdList, ComputeShader, ComputeShader.GetComputeShader(), Parameter);

	RHICmdList.DispatchComputeShader(SizeX, SizeY, 1);
	UseGlobalShader_RenderThread(RHICmdList, FeatureLevel, OutputRenderTargetResource, Texture->GetTexture2D());
}

void UGlobalTestShaderBlueprintLibrary::UseComputeShader(UTextureRenderTarget2D* OutputRenderTarget, AActor* Actor)
{
	check(IsInGameThread());

	if (!OutputRenderTarget)
		return;
	if (!Actor)
		return;

	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();

	UWorld* World = Actor->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();

	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[TextureRenderTargetResource, FeatureLevel](FRHICommandListImmediate& RHICmdList)
		{
			UseComputeShader_RenderThread(RHICmdList, TextureRenderTargetResource, FeatureLevel);
		}
	);
}
