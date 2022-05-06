#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyTestFunctionLibrary.generated.h"

UCLASS(MinimalAPI, meta = (ScriptName = "MyTestShaderLibrary"))
class UGlobalTestShaderBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "GlobalShaderTest", meta = (WorldContext = "WorldContextObject"))
    static void DrawGlobalTestShaderRenderTarget(
        class UTextureRenderTarget2D* OutputRenderTarget,
        AActor* Actor
    );
};