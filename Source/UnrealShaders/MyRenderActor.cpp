#include "MyRenderActor.h"
#include "MyCustomVS.h"
#include "MyCustomPS.h"
#include <CommonRenderResources.h>

// Sets default values
AMyRenderActor::AMyRenderActor()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyRenderActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMyRenderActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DrawMyActor();
}

void AMyRenderActor::DrawMyActor()
{
//    ENQUEUE_RENDER_COMMAND(FDrawMyActor)(
//        [this](FRHICommandListImmediate& RHICmdList)
//        {
//            RenderWithMyGlobalShader(RHICmdList);
//        });
}

void AMyRenderActor::RenderWithMyGlobalShader(FRHICommandListImmediate& RHICmdList)
{
    //TShaderMapRef<FMyCustomVS> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    //TShaderMapRef<FMyCustomPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

    //FGraphicsPipelineStateInitializer GraphicsPSOInit;
    //RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);

    //GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GEmptyVertexDeclaration.VertexDeclarationRHI;
    //GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
    //GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
    //GraphicsPSOInit.PrimitiveType = PT_TriangleList;

    //SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

    //// Example: Setting a color parameter for the pixel shader
    //PixelShader->SetColor(RHICmdList, FLinearColor::Red);
}
