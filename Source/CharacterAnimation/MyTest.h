// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "ScreenRendering.h"

class FMyTestVS : public FGlobalShader
{
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestVS, Global, CHARACTERANIMATION_API);

    FMyTestVS() { }
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
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestPS, Global, CHARACTERANIMATION_API);

    FMyTestPS() { }
    FMyTestPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
    }
    static bool ShouldCache(EShaderPlatform Platform)
    {
        return true;
    }
};