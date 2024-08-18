#include "MyCustomRenderingPass.h"
#include "GlobalShader.h"
#include "PipelineStateCache.h"
#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"
#include "../UnrealShaders/MyCustomVS.h"
#include "../UnrealShaders/MyCustomPS.h"

void FMyCustomRenderingPass::Execute_RenderThread(FRDGBuilder& GraphBuilder, FRHICommandListImmediate& RHICmdList)
{
    //// Set up and bind shaders
    //TShaderMapRef<FMyCustomVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    //TShaderMapRef<FMyCustomPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

    //FGraphicsPipelineStateInitializer GraphicsPSOInit;
    //RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
    //GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GetVertexDeclarationFVector4();
    //GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
    //GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
    //GraphicsPSOInit.PrimitiveType = PT_TriangleList;

    //SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit, 0); // Ensure to add the StencilRef parameter

    //// Set any shader parameters here
    //FLinearColor MyColor(0.0f, 1.0f, 0.0f); // Green color
    //PixelShader->SetColor(RHICmdList, MyColor);

    //// Execute drawing commands
    //RHICmdList.DrawPrimitive(0, 1, 1);
}
