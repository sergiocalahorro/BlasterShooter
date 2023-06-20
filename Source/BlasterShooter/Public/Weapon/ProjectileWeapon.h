// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// BlasterShooter
#include "BaseWeapon.h"

#include "ProjectileWeapon.generated.h"

// Forward declarations - BlasterShooter
class UDataAsset_ProjectileData;

UCLASS()
class BLASTERSHOOTER_API AProjectileWeapon : public ABaseWeapon
{
	GENERATED_BODY()

#pragma region WEAPON

public:

	/** Fire weapon */
	virtual void Fire(const FVector& HitTarget) override;

protected:

	/** Projectile's data asset */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Weapon|Projectile")
	TObjectPtr<UDataAsset_ProjectileData> ProjectileDataAsset;

#pragma endregion WEAPON
};
