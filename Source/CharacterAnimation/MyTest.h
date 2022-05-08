// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "ScreenRendering.h"

class FMyTest : public FGlobalShader
{
protected:
    FMyTest() {}
    FMyTest(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
    }

public:

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return true;
	}

protected:
};

class FMyTestVS : public FMyTest
{
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestVS, Global, CHARACTERANIMATION_API);
};

class FMyTestPS : public FMyTest
{
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestPS, Global, CHARACTERANIMATION_API);
};

class FMyTestCS : public FGlobalShader
{
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestCS, Global, CHARACTERANIMATION_API);

protected:
    FMyTestCS() { }
    FMyTestCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
        OutputTexture.Bind(Initializer.ParameterMap, TEXT("OutputTexture"));
	}

public:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
		return GetMaxSupportedFeatureLevel(Parameters.Platform) >= ERHIFeatureLevel::SM5;
	};
	static bool ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}

    FORCEINLINE const FShaderResourceParameter& GetOutputTexture() { return OutputTexture; }

protected:
    LAYOUT_FIELD(FShaderResourceParameter, OutputTexture);
};