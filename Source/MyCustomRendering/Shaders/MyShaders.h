#pragma once

#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "ScreenPass.h"

BEGIN_SHADER_PARAMETER_STRUCT(FFresnelShaderParameters, )
    SHADER_PARAMETER_TEXTURE(Texture2D, BaseTexture)
    SHADER_PARAMETER_SAMPLER(SamplerState, BaseTextureSampler)
    SHADER_PARAMETER(FLinearColor, HighlightColor)
    SHADER_PARAMETER(float, FresnelExponent)
    SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
    SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTexture)
    RENDER_TARGET_BINDING_SLOTS()  // Add this line
END_SHADER_PARAMETER_STRUCT()

class FFresnelShaderPS : public FGlobalShader
{
public:
    DECLARE_GLOBAL_SHADER(FFresnelShaderPS);
    SHADER_USE_PARAMETER_STRUCT(FFresnelShaderPS, FGlobalShader);

    using FParameters = FFresnelShaderParameters;
};

