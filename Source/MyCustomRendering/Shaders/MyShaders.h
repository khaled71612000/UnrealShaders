#pragma once

#include "GlobalShader.h"
#include "ScreenPass.h"


//FUVMaskShaderParameters:
//This structure defines the parameters for the FUVMaskShaderPS shader.
//SHADER_PARAMETER_RDG_TEXTURE : These parameters allow the shader to access textures.
//SHADER_PARAMETER_SAMPLER : Defines a sampler state for accessing texture data.
//SHADER_PARAMETER_STRUCT : This is a struct parameter for passing viewport information.
//RENDER_TARGET_BINDING_SLOTS : Used to define render target outputs for the shader.

//taking in a texture of the scene color, a sampler, a struct containing data about our view,
//and binding slots for our outputs.FUVMaskShaderParameters additionally takes a texture of ints for our custom stencil data,
//and FCombineShaderParameters takes an extra input texture(the result of the previous pass) and a color.

BEGIN_SHADER_PARAMETER_STRUCT(FUVMaskShaderParameters, )
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTexture)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColor)
	SHADER_PARAMETER_RDG_TEXTURE_SRV(Texture2D<uint2>, InputStencilTexture)
	SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)
	SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, ViewParams)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

class FUVMaskShaderPS : public FGlobalShader {
public:
	DECLARE_EXPORTED_SHADER_TYPE(FUVMaskShaderPS, Global,);
	using FParameters = FUVMaskShaderParameters;

	//SHADER_USE_PARAMETER_STRUCT: This macro associates the shader with the defined parameter structure.
	SHADER_USE_PARAMETER_STRUCT(FUVMaskShaderPS, FGlobalShader);
};

//FCombineShaderParameters:
//
//Similar to FUVMaskShaderParameters, this structure defines the parameters for the FCombineShaderPS shader.
//SHADER_PARAMETER : This specifies a linear color parameter.
//Other parameters : They are used similarly as in FUVMaskShaderParameters.


BEGIN_SHADER_PARAMETER_STRUCT(FCombineShaderParameters, )
	SHADER_PARAMETER(FLinearColor, Color)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColor)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InputTexture)
	SHADER_PARAMETER_SAMPLER(SamplerState, InputSampler)
	SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, ViewParams)
	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

class FCombineShaderPS : public FGlobalShader {
public:
	DECLARE_EXPORTED_SHADER_TYPE(FCombineShaderPS, Global,);
	using FParameters = FCombineShaderParameters;
	SHADER_USE_PARAMETER_STRUCT(FCombineShaderPS, FGlobalShader);
};