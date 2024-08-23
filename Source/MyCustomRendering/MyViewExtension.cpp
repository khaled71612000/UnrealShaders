#include "MyViewExtension.h"
#include "GlobalShader.h"
#include "./Shaders/MyShaders.h"
#include <PixelShaderUtils.h>
#include "PostProcess/PostProcessing.h" // For FPostProcessingInputs
#include "SceneView.h" // For FViewInfo

void FMyViewExtension::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
    // Custom logic to setup the view family
}

void FMyViewExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
    // Custom logic to setup the view
}

void FMyViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
    // Custom logic to handle the beginning of the render view family
}

void FMyViewExtension::PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily)
{
    // Custom logic before rendering the view family
}

void FMyViewExtension::PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView)
{
    // Custom logic before rendering the view
}
void FMyViewExtension::PostRenderBasePass_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView)
{
}

void FMyViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs)
{
    // Check if the CustomDepthTexture format is not PF_DepthStencil.
    // This check was necessary in UE5 because certain post-processing passes or custom render passes
    // may require the depth buffer to have a specific format. If the format is not as expected,
    // the function exits early to prevent rendering issues. This check may not be required in other
    // versions, but it's kept here for backward compatibility and safety.
    if ((*Inputs.SceneTextures)->CustomDepthTexture->Desc.Format != PF_DepthStencil) return;

    // Ensure that the View is actually of type FViewInfo by checking the bIsViewInfo flag.
    // This is important because FViewInfo contains specific data and methods required for rendering.
    // Unlike dynamic_cast, which isn't possible here due to the lack of virtual functions in FViewInfo,
    // this check confirms the type of View safely in a performance-efficient manner.
    checkSlow(View.bIsViewInfo);

    // Get the viewport rectangle of the view, which defines the area of the screen being rendered to.
    // This is later used to correctly align the rendering and post-processing operations to the correct
    // screen space region.
    const FIntRect Viewport = static_cast<const FViewInfo&>(View).ViewRect;

    // Create a texture representation for the scene color, using the provided viewport.
    // FScreenPassTexture is a wrapper around the scene color texture that simplifies the use of this texture
    // in screen-space passes.
    FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, Viewport);

    // Retrieve the global shader map, which contains all the shaders compiled and available for use
    // at the current RHI feature level. This is necessary to bind the correct shader during rendering.
    FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);

    // Begin a new render event scope named "My Render Pass". This is useful for GPU debugging and profiling,
    // allowing developers to identify which part of the render graph is being executed.
    RDG_EVENT_SCOPE(GraphBuilder, "My Render Pass");

    // Viewport parameters
    // These parameters are calculated for the scene color texture and will be used later by shaders
    // to correctly map UVs and screen-space coordinates.
    const FScreenPassTextureViewport SceneColorTextureViewport(SceneColor);
    const FScreenPassTextureViewportParameters SceneTextureViewportParams = GetTextureViewportParameters(SceneColorTextureViewport);

    // Render targets
    // Create a copy of the scene color texture, which will be used in subsequent passes. 
    // This copy allows modifications without affecting the original scene color data.
    FScreenPassRenderTarget SceneColorCopyRenderTarget;
    SceneColorCopyRenderTarget.Texture = GraphBuilder.CreateTexture((*Inputs.SceneTextures)->SceneColorTexture->Desc, TEXT("Scene Color Copy"));

    // Create a UV mask render target. This target will be used to store the results of a shader pass
    // that generates a UV mask based on custom criteria, such as object positions or screen-space effects.
    FScreenPassRenderTarget UVMaskRenderTarget;
    UVMaskRenderTarget.Texture = GraphBuilder.CreateTexture((*Inputs.SceneTextures)->SceneColorTexture->Desc, TEXT("UV Mask"));

    // Shader setup for the UV mask pass
    // Bind the UV mask pixel shader and allocate parameters needed for this pass. The shader will use
    // the scene color texture and the custom depth-stencil buffer to generate the UV mask.
    TShaderMapRef<FUVMaskShaderPS> UVMaskPixelShader(GlobalShaderMap);
    FUVMaskShaderPS::FParameters* UVMaskParameters = GraphBuilder.AllocParameters<FUVMaskShaderPS::FParameters>();
    UVMaskParameters->SceneColor = (*Inputs.SceneTextures)->SceneColorTexture;
    UVMaskParameters->InputStencilTexture = GraphBuilder.CreateSRV(FRDGTextureSRVDesc::CreateWithPixelFormat((*Inputs.SceneTextures)->CustomDepthTexture, PF_X24_G8));
    UVMaskParameters->InputSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
    UVMaskParameters->ViewParams = SceneTextureViewportParams;
    UVMaskParameters->RenderTargets[0] = UVMaskRenderTarget.GetRenderTargetBinding();
    UVMaskParameters->RenderTargets[1] = SceneColorCopyRenderTarget.GetRenderTargetBinding();

    // Add a full-screen pass to the render graph using the UV mask pixel shader.
    // This pass will process the entire screen and output the UV mask and the scene color copy.
    FPixelShaderUtils::AddFullscreenPass(
        GraphBuilder,
        GlobalShaderMap,
        FRDGEventName(TEXT("UV Mask")),
        UVMaskPixelShader,
        UVMaskParameters,
        Viewport);

    // Shader setup for the combination pass
    // Bind the combine pixel shader, which takes the UV mask and scene color copy as inputs.
    // This shader applies a color highlight based on the UV mask and writes the result back to the scene color.
    TShaderMapRef<FCombineShaderPS> CombinePixelShader(GlobalShaderMap);
    FCombineShaderPS::FParameters* CombineParameters = GraphBuilder.AllocParameters<FCombineShaderPS::FParameters>();
    CombineParameters->SceneColor = SceneColorCopyRenderTarget.Texture;
    CombineParameters->InputTexture = UVMaskRenderTarget.Texture;
    CombineParameters->InputSampler = TStaticSamplerState<SF_Point, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI();
    CombineParameters->Color = HighlightColor;
    CombineParameters->ViewParams = SceneTextureViewportParams;
    CombineParameters->RenderTargets[0] = FRenderTargetBinding(SceneColor.Texture, ERenderTargetLoadAction::ELoad);

    // Add another full-screen pass to the render graph using the combine pixel shader.
    // This pass merges the highlighted UV areas back into the scene color texture.
    FPixelShaderUtils::AddFullscreenPass(
        GraphBuilder,
        GlobalShaderMap,
        FRDGEventName(TEXT("Combine")),
        CombinePixelShader,
        CombineParameters,
        Viewport);
}

FScreenPassTextureViewportParameters FMyViewExtension::GetTextureViewportParameters(const FScreenPassTextureViewport& InViewport) {
    // Initialize the viewport parameters based on the input viewport, which represents a section of the screen.
    const FVector2f Extent(InViewport.Extent);
    const FVector2f ViewportMin(InViewport.Rect.Min.X, InViewport.Rect.Min.Y);
    const FVector2f ViewportMax(InViewport.Rect.Max.X, InViewport.Rect.Max.Y);
    const FVector2f ViewportSize = ViewportMax - ViewportMin;

    FScreenPassTextureViewportParameters Parameters;

    // If the viewport is valid (i.e., not empty), calculate various parameters
    // needed for rendering, such as screen-space to viewport transformations.
    if (!InViewport.IsEmpty()) {
        Parameters.Extent = FVector2f(Extent);
        Parameters.ExtentInverse = FVector2f(1.0f / Extent.X, 1.0f / Extent.Y);

        // These parameters are used to convert screen-space coordinates to the current viewport.
        Parameters.ScreenPosToViewportScale = FVector2f(0.5f, -0.5f) * ViewportSize;
        Parameters.ScreenPosToViewportBias = (0.5f * ViewportSize) + ViewportMin;

        // Store the viewport's min and max coordinates.
        Parameters.ViewportMin = InViewport.Rect.Min;
        Parameters.ViewportMax = InViewport.Rect.Max;

        // Store the viewport size and its inverse, which is useful for normalized texture coordinates.
        Parameters.ViewportSize = ViewportSize;
        Parameters.ViewportSizeInverse = FVector2f(1.0f / Parameters.ViewportSize.X, 1.0f / Parameters.ViewportSize.Y);

        // Calculate UV coordinates for the viewport.
        Parameters.UVViewportMin = ViewportMin * Parameters.ExtentInverse;
        Parameters.UVViewportMax = ViewportMax * Parameters.ExtentInverse;

        // Calculate the size of the UV viewport and its inverse.
        Parameters.UVViewportSize = Parameters.UVViewportMax - Parameters.UVViewportMin;
        Parameters.UVViewportSizeInverse = FVector2f(1.0f / Parameters.UVViewportSize.X, 1.0f / Parameters.UVViewportSize.Y);

        // Bilinear filtering adjustments to avoid sampling artifacts at the edges of the viewport.
        Parameters.UVViewportBilinearMin = Parameters.UVViewportMin + 0.5f * Parameters.ExtentInverse;
        Parameters.UVViewportBilinearMax = Parameters.UVViewportMax - 0.5f * Parameters.ExtentInverse;
    }

    // Return the computed parameters, which will be used in shaders to correctly map and render the viewport.
    return Parameters;
}