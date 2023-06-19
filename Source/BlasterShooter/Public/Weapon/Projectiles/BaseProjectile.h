// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseProjectile.generated.h"

class UBoxComponent;

UCLASS()
class BLASTERSHOOTER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	ABaseProjectile();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

	/** Box collision */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxCollision;

#pragma endregion COMPONENTS
	
};
