#pragma once

#include "GlobalShader.h"
//Provides utilities for handling shader parameters, such as binding them to the shader's parameter map.
#include "ShaderParameterUtils.h"
//Contains static state classes for rendering hardware interface (RHI) states like blend states, rasterizer states, and depth-stencil states, which are often used when setting up shaders.
#include "RHIStaticStates.h"

class FMyCustomPS : public FGlobalShader
{
    //DECLARE_EXPORTED_SHADER_TYPE(FMyCustomPS, Global, UNREALSHADERS_API);

public:
    FShaderParameter MyColorParameter;

    FMyCustomPS() { }

    FMyCustomPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
        MyColorParameter.Bind(Initializer.ParameterMap, TEXT("MyColor"));
    }

    static bool ShouldCache(EShaderPlatform Platform)
    {
        if (Platform == SP_OPENGL_ES3_1_ANDROID ||   // Android ES3.1
            Platform == SP_METAL ||                 // iOS Metal
            Platform == SP_VULKAN_ES3_1_ANDROID)    // Android Vulkan ES3.1
        {
            return false;  // Do not compile this shader for mobile platforms
        }

        // Otherwise, compile for all other platforms (e.g., PC)
        return true;
    }

    // Modify the compilation environment for this shader
    //You can add preprocessor definitions, set flags, or modify the compiler's behavior for this shader. The example simply calls the base class implementation, but additional custom settings could be added here.
    static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
    {
        FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
        // Additional modifications
        OutEnvironment.SetDefine(TEXT("MY_DEFINE"), 1);
    }

    // Serialize the shader parameters (important for cooked builds)
    //virtual bool Serialize(FArchive& Ar) override
    //{
    //    bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
    //    Ar << MyColorParameter;
    //    return bShaderHasOutdatedParameters;
    //}

    // Function to set the color parameter at runtime
    void SetColor(FRHICommandList& RHICmdList, const FLinearColor& Color)
    {
        //TShaderMapRef<FMyCustomPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
        //SetShaderValue(RHICmdList, PixelShader.GetPixelShader(), MyColorParameter, Color);
        SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), MyColorParameter, Color);

        //FRHIPixelShader* ps = RHICmdList.GetBoundPixelShader();
        //SetShaderValue(RHICmdList, ps, MyColorParameter, Color);

    }

    //virtual ~FMyCustomPS() {}
};
