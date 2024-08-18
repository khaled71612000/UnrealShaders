#include "MyShaders.h"

IMPLEMENT_SHADER_TYPE(, FCombineShaderPS, TEXT("/MyCustomRenderingModule/MyCustomShader.usf"), TEXT("CombineMainPS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FUVMaskShaderPS, TEXT("/MyCustomRenderingModule/MyCustomShader.usf"), TEXT("UVMaskMainPS"), SF_Pixel);