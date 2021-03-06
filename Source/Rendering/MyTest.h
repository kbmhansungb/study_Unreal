// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "ScreenRendering.h"
#include "Runtime/RenderCore/Public/ShaderParameterStruct.h"
#include "Runtime/RenderCore/Public/ShaderParameterMacros.h"

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

BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FUniformBufferTest, )
	SHADER_PARAMETER(float, Num)
END_GLOBAL_SHADER_PARAMETER_STRUCT()

class FMyTestCS : public FGlobalShader
{
	DECLARE_GLOBAL_SHADER(FMyTestCS);

	SHADER_USE_PARAMETER_STRUCT(FMyTestCS, FGlobalShader)
	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_UAV(RWTexture2D<float4>, OutputTexture)
		SHADER_PARAMETER_STRUCT_REF(FUniformBufferTest, UniformBufferTest)
	END_SHADER_PARAMETER_STRUCT()

protected:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) {
		return GetMaxSupportedFeatureLevel(Parameters.Platform) >= ERHIFeatureLevel::SM5;
	};
	static bool ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}
};