#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"
#include "ShaderParameterStruct.h"
#include "ScreenPass.h"

//In Unreal 5, the ISceneViewExtension class has been enhanced with overloaded functions which take an FRDGBuilder reference rather than FRHICommandListImmediate

//class ISceneViewExtension {
//    ...
//        virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) {};
//    virtual ENGINE_API void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView);
//    ...
//}
//
//// From Engine\Source\Runtime\Engine\Private\SceneViewExtension.cpp
//void ISceneViewExtension::PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) {
//    AddPass(GraphBuilder, RDG_EVENT_NAME("PreRenderView_RenderThread"), [this, &InView](FRHICommandListImmediate& RHICmdList) {
//        PreRenderView_RenderThread(RHICmdList, InView);
//        });
//}


#ifdef MYCUSTOMRENDERING_EXPORTS
#define MYCUSTOMRENDERING_API __declspec(dllexport)
#else
#define MYCUSTOMRENDERING_API __declspec(dllimport)
#endif


class MYCUSTOMRENDERING_API  FMyViewExtension : public FSceneViewExtensionBase
{
public:

    FMyViewExtension(const FAutoRegister& AutoRegister)
        : FSceneViewExtensionBase(AutoRegister)
    {
    }

    // Constructor with a custom color parameter that also initializes the base class
    FMyViewExtension(const FAutoRegister& AutoRegister, FLinearColor CustomColor)
        : FSceneViewExtensionBase(AutoRegister), HighlightColor(CustomColor)
    {
    }

    FLinearColor HighlightColor;

    // Override necessary methods from FSceneViewExtensionBase
    virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
    virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
    virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
    virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) override;
    virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override;

    virtual void PostRenderBasePass_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override;
    virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& View, const FPostProcessingInputs& Inputs) override;
    FScreenPassTextureViewportParameters GetTextureViewportParameters(const FScreenPassTextureViewport& InViewport);
};
