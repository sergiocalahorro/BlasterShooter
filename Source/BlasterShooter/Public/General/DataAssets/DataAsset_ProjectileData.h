// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// BlasterShooter
#include "General/Structs/Data/ProjectileData.h"

#include "DataAsset_ProjectileData.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UDataAsset_ProjectileData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Projectile's data */
	UPROPERTY(EditDefaultsOnly)
	FProjectileData ProjectileData;
};
