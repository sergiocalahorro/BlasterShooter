// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// BlasterShooter
#include "General/Structs/Data/WeaponData.h"

#include "DataAsset_WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UDataAsset_WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Weapon's data */
	UPROPERTY(EditDefaultsOnly)
	FWeaponData WeaponData;
};
