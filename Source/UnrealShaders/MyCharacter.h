#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../MyCustomRendering/MyViewExtension.h"
#include "MyCharacter.generated.h"

UCLASS()
class UNREALSHADERS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

	TSharedPtr<class FMyViewExtension, ESPMode::ThreadSafe> MyViewExtension;

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
