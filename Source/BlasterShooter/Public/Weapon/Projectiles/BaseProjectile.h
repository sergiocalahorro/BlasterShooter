// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "General/DataAssets/DataAsset_ProjectileData.h"

#include "BaseProjectile.generated.h"

// Forward declarations - Unreal Engine
class UBoxComponent;
class UProjectileMovementComponent;

// Forward declarations - BlasterShooter
class UDataAsset_ProjectileData;

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

private:

	/** Box collision */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UBoxComponent> BoxCollision;

	/** Projectile's movement component */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	/** Tracer component */
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> TracerComponent;

#pragma endregion COMPONENTS

#pragma region PROJECTILE

public:

	/** Initialize projectile's values */
	void InitializeProjectile(const UDataAsset_ProjectileData* ProjectileDataAsset);

#pragma endregion PROJECTILE
	
};
