// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

public:

	/** Weapon's static mesh */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh;
	
	/** Weapon's skeletal mesh */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
};