// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "ScreenRendering.h"

class FMyTestVS : public FGlobalShader
{
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestVS, Global, /*MYMODULE_API*/);

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
    DECLARE_EXPORTED_SHADER_TYPE(FMyTestPS, Global, /*MYMODULE_API*/);

    //FShaderParameter MyColorParameter;

    FMyTestPS() { }
    FMyTestPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
        // struct에 parameter를 할당하지 않으면, unbound error를 발생시킵니다.
        //MyColorParameter.Bind(Initializer.ParameterMap, TEXT("MyColor"), SPF_Mandatory);
    }

    //static void ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
    //{
    //    FGlobalShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
    //    OutEnvironment.SetDefine(TEXT("MY_DEFINE"), 1);
    //}

    static bool ShouldCache(EShaderPlatform Platform)
    {
        return true;
    }

    //virtual bool Serialize(FArchive& Ar) override
    //{
    //    bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
    //    Ar << MyColorParameter;
    //    return bShaderHasOutdatedParameters;
    //}

    //void SetColor(FRHICommandList& RHICmdList, const FLinearColor& Color)
    //{
    //    SetShaderValue(RHICmdList, GetPixelShader(), MyColorParameter, Color);
    //}
};