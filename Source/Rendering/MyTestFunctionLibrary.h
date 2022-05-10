#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyTestFunctionLibrary.generated.h"

class UTextureRenderTarget2D;
class UTexture2D;

UCLASS(MinimalAPI, meta = (ScriptName = "MyTestShaderLibrary"))
class UGlobalTestShaderBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "GlobalShaderTest", meta = (WorldContext = "WorldContextObject"))
    static void UseGlobalShader(
        UTextureRenderTarget2D* OutputRenderTarget,
        UTexture* SrcTexture,
        AActor* Actor
    );

    //UFUNCTION(BlueprintCallable, Category = "GlobalShaderTest", meta = (WorldContext = "WorldContextObject"))
    //static void UseComputeShader(
    //    UTextureRenderTarget2D* OutputRenderTarget,
    //    AActor* Actor
    //);
};