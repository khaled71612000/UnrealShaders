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
    if ((*Inputs.SceneTextures)->CustomDepthTexture->Desc.Format != PF_DepthStencil) return;

    checkSlow(View.bIsViewInfo);

    const FIntRect Viewport = static_cast<const FViewInfo&>(View).ViewRect;
    FScreenPassTexture SceneColor((*Inputs.SceneTextures)->SceneColorTexture, Viewport);

    if (!SceneColor.Texture)
    {
        return;
    }

    FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);

    RDG_EVENT_SCOPE(GraphBuilder, "My Fresnel Pass");

    TShaderMapRef<FFresnelShaderPS> FresnelShader(GlobalShaderMap);
    FFresnelShaderPS::FParameters* Parameters = GraphBuilder.AllocParameters<FFresnelShaderPS::FParameters>();

    Parameters->BaseTexture = View.Family->RenderTarget->GetRenderTargetTexture();
    Parameters->BaseTextureSampler = TStaticSamplerState<SF_Trilinear>::GetRHI();
    Parameters->HighlightColor = HighlightColor;
    Parameters->FresnelExponent = 10.0f;

    Parameters->View = View.ViewUniformBuffer;
    Parameters->SceneColorTexture = SceneColor.Texture;

    // Bind the output render target to the graph parameters
    FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(
        FRDGTextureDesc::Create2D(Viewport.Size(), PF_B8G8R8A8, FClearValueBinding::Black, TexCreate_RenderTargetable),
        TEXT("OutputRenderTarget"));

    Parameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ELoad);

    // Add fullscreen pass to the render graph using the Fresnel pixel shader
    FPixelShaderUtils::AddFullscreenPass(
        GraphBuilder,
        GlobalShaderMap,
        RDG_EVENT_NAME("FresnelEffect"),
        FresnelShader,
        Parameters,
        Viewport
    );
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