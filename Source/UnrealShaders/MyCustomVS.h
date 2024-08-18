#pragma once

#include "GlobalShader.h"  // Include the necessary header for global shaders
#include "ScreenPass.h"

// Declare a new class for the vertex shader
class FMyCustomVS : public FGlobalShader
{
    // This macro declares the necessary metadata and functions for the shader type. It registers the shader type with the engine's shader management system.
    //DECLARE_EXPORTED_SHADER_TYPE(FMyCustomVS, Global, UNREALSHADERS_API);
public:
    // Default constructor
    FMyCustomVS() { }

    // Constructor used during shader compilation
    // It initializes the shader using metadata provided by Unreal's shader system.
    FMyCustomVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
    }

    // Determines whether this shader should be compiled for a specific platform
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
};
