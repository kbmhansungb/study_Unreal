//// Bum moo, Kim
//// 2022-05-07
//
//#include "MyRDGGraph.h"
//
//#include <Engine/World.h>
//#include <RHIResources.h>
//#include <GlobalShader.h>
//#include <renderGraphBuilder.h>
//
//struct FMyShaderParameters
//{
//};
//
//class FMyShaderCS : public FGlobalShader
//{
//    DECLARE_GLOBAL_SHADER(FMyShaderCS);
//
//    FMyShaderCS() { }
//    FMyShaderCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
//		: FGlobalShader(Initializer)
//	{
//	}
//
//	static bool ShouldCache(EShaderPlatform Platform)
//	{
//		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
//	}
//
//    using FParameters = FMyShaderParameters;
//};
//IMPLEMENT_SHADER_TYPE(, FMyShaderCS, TEXT("/ShaderAutogen/MyRDGShader.usf"), TEXT("MainCS"), SF_Vertex);
//
//static void MyRDG_RenderingThread(FRHICommandList& RHICmdList, ERHIFeatureLevel::Type FeatureLevel)
//{
//	auto GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
//	TShaderMapRef<FMyShaderCS> ComputeShader(GlobalShaderMap);
//
//    RHICmdList.SetComputeShader(ComputeShader.GetComputeShader());
//	RHICmdList.DispatchComputeShader(8, 8, 1);
//}
//
//void URDGFunctionLibrary::RegistRDGFunction(AActor* Actor)
//{
//	if (Actor == nullptr)
//		return;
//	
//	UWorld* World = Actor->GetWorld();
//	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
//
//	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
//		[FeatureLevel]
//	(FRHICommandListImmediate& RHICmdList)
//		{
//			FRDGBuilder GraphBuilder(RHICmdList);
//
//			FMyShaderCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FMyShaderCS::FParameters>();
//
//			GraphBuilder.AddPass(
//				RDG_EVENT_NAME("MyRDGComputeShader"),
//				ERDGPassFlags::None,
//				[FeatureLevel](FRHICommandListImmediate& RHICmdList)
//				{
//					MyRDG_RenderingThread(RHICmdList, FeatureLevel);
//				});
//
//			GraphBuilder.Execute();
//		}
//	);
//}
