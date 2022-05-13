// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTest.h"

// VS PS Shader
IMPLEMENT_SHADER_TYPE(, FMyTestVS, TEXT("/ShaderAutogen/MyTest.usf"), TEXT("MainVS"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FMyTestPS, TEXT("/ShaderAutogen/MyTest.usf"), TEXT("MainPS"), SF_Pixel);

// Uniform buffer
IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FUniformBufferTest, "UniformBufferTest");

// Compute Shader
IMPLEMENT_SHADER_TYPE(, FMyTestCS, TEXT("/ShaderAutogen/MyComputeShader.usf"), TEXT("MainCS"), SF_Compute);