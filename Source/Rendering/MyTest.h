// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "ScreenRendering.h"

class FMyTestVS : public FGlobalShader
{
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestVS, Global, RENDERING_API);

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
	DECLARE_EXPORTED_SHADER_TYPE(FMyTestPS, Global, RENDERING_API);

protected:
	FMyTestPS() {}
	FMyTestPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
	}

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