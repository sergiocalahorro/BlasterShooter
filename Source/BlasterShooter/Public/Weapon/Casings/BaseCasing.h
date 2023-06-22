// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// BlasterShooter
#include "General/Structs/Data/CasingData.h"

#include "BaseCasing.generated.h"

// Forward declarations - BlasterShooter
class UDataAsset_CasingData;

UCLASS()
class BLASTERSHOOTER_API ABaseCasing : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	ABaseCasing();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:

	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

private:

	/** Casing's static mesh */
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CasingMesh;

#pragma endregion COMPONENTS

#pragma region CASING

public:

	/** Initialize casing's values */
	void InitializeCasing(const UDataAsset_CasingData* CasingDataAsset);

protected:

	/** Handle casing's hit */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:

	/** Launch casing when spawned */
	void LaunchCasing(float Impulse);
	
private:

	/** Casing's data */
	UPROPERTY()
	FCasingData CasingData;

#pragma endregion CASING

};
