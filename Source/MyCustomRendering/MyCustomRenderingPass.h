#pragma once

#include "CoreMinimal.h"
#include "RenderGraphUtils.h"

class FMyCustomRenderingPass
{
public:
    static void Execute_RenderThread(FRDGBuilder& GraphBuilder, FRHICommandListImmediate& RHICmdList);
};

