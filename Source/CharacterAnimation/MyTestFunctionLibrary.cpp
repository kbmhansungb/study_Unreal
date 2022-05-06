#include "MyTestFunctionLibrary.h"
#include "MyTest.h"

#include "TextureResource.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "GlobalShader.h"

struct FMyBufferStruct
{
	FVector4f Position;
};

class FMyVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;

	virtual void InitRHI() override
	{
		FVertexDeclarationElementList Elements;
		uint32 Stride = sizeof(FMyBufferStruct);
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FMyBufferStruct, Position), VET_Float4, 0, Stride));
		VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
	}

	virtual void ReleaseRHI() override
	{
		VertexDeclarationRHI->Release();
	}
};

class FMyVertexBuffer : public FVertexBuffer
{
public:
	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(L"MyTest");
		VertexBufferRHI = RHICreateVertexBuffer(sizeof(FMyBufferStruct) * 4, BUF_Static, CreateInfo);
		void* VoidPtr = RHILockBuffer(VertexBufferRHI, 0, sizeof(FMyBufferStruct) * 4, RLM_WriteOnly);

		FMyBufferStruct* Vertices = reinterpret_cast<FMyBufferStruct*>(VoidPtr);
		Vertices[0].Position = FVector4f(-1.0f, 1.0f, 0.0f, 1.0f);
		Vertices[1].Position = FVector4f(1.0f, 1.0f, 0.0f, 1.0f);
		Vertices[2].Position = FVector4f(-1.0f, -1.0f, 0.0f, 1.0f);
		Vertices[3].Position = FVector4f(1.0f, -1.0f, 0.0f, 1.0f);

		RHIUnlockBuffer(VertexBufferRHI);
	}
};

class FMyIndexBuffer : public FIndexBuffer
{
public:
	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo(L"MyTest");
		using TYPE = uint16;
		const uint32 Stride = sizeof(TYPE);
		const uint32 Size = Stride * 6;
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

TGlobalResource<FMyVertexBuffer> VertexBuffer;
TGlobalResource<FMyIndexBuffer> IndexBuffer;


static void DrawMyShader_RenderThread (
	FRHICommandListImmediate& RHICmdList,
	ERHIFeatureLevel::Type FeatureLevel,
	FTextureRenderTargetResource* OutputRenderTargetResource
)
{
	check(IsInRenderingThread());

	FRHIRenderPassInfo RPInfo(
		OutputRenderTargetResource->GetRenderTargetTexture(),
		ERenderTargetActions::DontLoad_Store
	);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("DrawGlobalTest"));
	
	auto GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
	TShaderMapRef<FMyTestVS> VertexShader(GlobalShaderMap);
	TShaderMapRef<FMyTestPS> PixelShader(GlobalShaderMap);

	FMyVertexDeclaration VertexDec;
	VertexDec.InitRHI();

	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

	GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
	GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
	GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();

	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = VertexDec.VertexDeclarationRHI;
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
	GraphicsPSOInit.PrimitiveType = PT_TriangleList;

	SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, EApplyRendertargetOption::ForceApply);

	RHICmdList.SetStreamSource(0, VertexBuffer.VertexBufferRHI, 0);

	const uint32 VerticiesNum = 4;
	const uint32 TriangleNum = 2;
	RHICmdList.DrawIndexedPrimitive(IndexBuffer.IndexBufferRHI, 0, 0, VerticiesNum, 0, TriangleNum, 1);

	RHICmdList.EndRenderPass();
}

void UGlobalTestShaderBlueprintLibrary::DrawGlobalTestShaderRenderTarget(
	UTextureRenderTarget2D* OutputRenderTarget,
	AActor* Actor
)
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
			[FeatureLevel, TextureRenderTargetResource]
			(FRHICommandListImmediate& RHICmdList)
			{
				DrawMyShader_RenderThread(RHICmdList, FeatureLevel, TextureRenderTargetResource);
			}
		);
}