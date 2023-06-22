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
	
	/** Called when this actor is explicitly being destroyed during gameplay or in the editor, not called during level streaming or gameplay ending */
	virtual void Destroyed() override;
	
protected:
	
	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	
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

protected:

	/** Handle projectile's hit */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:

	/** Projectile's data */
	UPROPERTY()
	FProjectileData ProjectileData;
	
#pragma endregion PROJECTILE
	
};
