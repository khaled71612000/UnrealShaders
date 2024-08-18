#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRenderActor.generated.h"

UCLASS()
class UNREALSHADERS_API AMyRenderActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMyRenderActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DrawMyActor();
	void RenderWithMyGlobalShader(FRHICommandListImmediate& RHICmdList);
};
