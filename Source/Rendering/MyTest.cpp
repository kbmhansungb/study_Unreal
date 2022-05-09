// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest.h"

IMPLEMENT_SHADER_TYPE(, FMyTestVS, TEXT("/ShaderAutogen/MyTest.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FMyTestPS, TEXT("/ShaderAutogen/MyTest.usf"), TEXT("MainPS"), SF_Pixel);

//// Compute Shader
//IMPLEMENT_SHADER_TYPE(, FMyTestCS, TEXT("/ShaderAutogen/MyComputeShader.usf"), TEXT("MainCS"), SF_Compute);