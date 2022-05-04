#include "MyTestFunctionLibrary.h"
#include "MyTest.h"

#include "TextureResource.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "GlobalShader.h"

class FMyVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;

	virtual void InitRHI() override
	{
		FVertexDeclarationElementList Elements;
		uint32 Stride = sizeof(FVector4);
		Elements.Add(FVertexElement(0, 0, VET_Float4, 0, Stride));
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
		VertexBufferRHI = RHICreateVertexBuffer(sizeof(FVector4) * 4, BUF_Static, CreateInfo);
		void* VoidPtr = RHILockVertexBuffer(VertexBufferRHI, 0, sizeof(FVector4) * 4, RLM_WriteOnly);
		FVector4* Vertices = reinterpret_cast<FVector4*>(VoidPtr);

		Vertices[0] = FVector4(-1.0f, 0.0f, 0.0f, 1.0f);
		Vertices[1] = FVector4(0.0f, 1.0f, 0.0f, 1.0f);
		Vertices[2] = FVector4(1.0f, 0.0f, 0.0f, 1.0f);
		RHIUnlockVertexBuffer(VertexBufferRHI);
	}
};

TGlobalResource<FMyVertexBuffer> MyVertexBuffer;

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

	FRHITexture* RHITexture = TextureRenderTargetResource->GetRenderTargetTexture();
	FRHIRenderPassInfo RPInfo(RHITexture, ERenderTargetActions::Clear_Store, TextureRenderTargetResource->TextureRHI, FExclusiveDepthStencil::DepthNop_StencilNop);

	FName TextureRenderTargetName = OutputRenderTarget->GetFName();
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
			[RPInfo, FeatureLevel, TextureRenderTargetName]
			(FRHICommandListImmediate& RHICmdList)
			{
				check(IsInRenderingThread());

				RHICmdList.BeginRenderPass(RPInfo, TEXT("DrawGlobalTest"));
				{
					auto GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
					TShaderMapRef<FMyTestVS> VertexShader(GlobalShaderMap);
					TShaderMapRef<FMyTestPS> PixelShader(GlobalShaderMap);

					FGraphicsPipelineStateInitializer GraphicsPSOInit;
					RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

					GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
					GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
					GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
					GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;

					FMyVertexDeclaration VertexDec;
					VertexDec.InitRHI();

					GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = VertexDec.VertexDeclarationRHI;
					GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
					GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();

					SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

					RHICmdList.SetStreamSource(0, MyVertexBuffer.VertexBufferRHI, 0);
					RHICmdList.DrawPrimitive(0, 3, 0);
				}
				RHICmdList.EndRenderPass();
			}
		);
}