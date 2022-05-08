#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyTestFunctionLibrary.generated.h"

class UTextureRenderTarget2D;

UCLASS(MinimalAPI, meta = (ScriptName = "MyTestShaderLibrary"))
class UGlobalTestShaderBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "GlobalShaderTest", meta = (WorldContext = "WorldContextObject"))
    static void UseGlobalShader(
        UTextureRenderTarget2D* OutputRenderTarget,
        AActor* Actor
    );

    UFUNCTION(BlueprintCallable, Category = "GlobalShaderTest", meta = (WorldContext = "WorldContextObject"))
    static void UseGlobalShader2(
        UTextureRenderTarget2D* OutputRenderTarget,
        UTexture2D* Texture2D,
        AActor* Actor
    );

    UFUNCTION(BlueprintCallable, Category = "GlobalShaderTest", meta = (WorldContext = "WorldContextObject"))
    static void UseComputeShader(
        UTextureRenderTarget2D* OutputRenderTarget,
        AActor* Actor
    );
};