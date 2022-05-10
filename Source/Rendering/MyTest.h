// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "ScreenRendering.h"
#include "Runtime/RenderCore/Public/ShaderParameterStruct.h"

class FMyTestVS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMyTestVS);

protected:
	FMyTestVS() {}
	FMyTestVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}
};

class FMyTestPS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMyTestPS);

	SHADER_USE_PARAMETER_STRUCT(FMyTestPS, FGlobalShader)
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_TEXTURE(Texture2D, SrcTexture)
		SHADER_PARAMETER_SAMPLER(SamplerState, SrcTextureSampler)
	END_SHADER_PARAMETER_STRUCT()

protected:
	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}
};

//class FMyTestCS : public FGlobalShader
//{
//    DECLARE_EXPORTED_SHADER_TYPE(FMyTestCS, Global, CHARACTERANIMATION_API);
//
//protected:
//    FMyTestCS() { }
//    FMyTestCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
//		: FGlobalShader(Initializer)
//	{
//        OutputTexture.Bind(Initializer.ParameterMap, TEXT("OutputTexture"));
//	}
//
//public:
//	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
//		return GetMaxSupportedFeatureLevel(Parameters.Platform) >= ERHIFeatureLevel::SM5;
//	};
//	static bool ShouldCache(EShaderPlatform Platform)
//	{
//		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
//	}
//
//    FORCEINLINE const FShaderResourceParameter& GetOutputTexture() { return OutputTexture; }
//
//protected:
//    LAYOUT_FIELD(FShaderResourceParameter, OutputTexture);
//};