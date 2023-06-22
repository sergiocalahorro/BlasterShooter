// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// BlasterShooter
#include "General/Enums/Weapon/WeaponType.h"

#include "WeaponData.generated.h"

// Forward declarations - Unreal Engine
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;

// Forward declarations - BlasterShooter
class ABaseProjectile;
class ABaseCasing;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

public:

	/** Weapon's shooting type: trace/projectile */
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType = EWeaponType::Trace;

	/** Weapon's static mesh */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh;
	
	/** Weapon's skeletal mesh */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	/** Character's animation montage to play when firing with this weapon */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	/** Montage's section name when firing from the hip */
	UPROPERTY(EditDefaultsOnly)
	FName AttackMontageHipSectionName;

	/** Montage's section name when firing while aiming */
	UPROPERTY(EditDefaultsOnly)
	FName AttackMontageAimSectionName;
	
	/** Weapon's attack sound */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> AttackSound;

	/** Weapon's attack VFX */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> AttackVfx;

	/** Weapon's animation to play when character is firing */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimationAsset> FireAnimation;
	
	/** Class of casing to spawn when the weapon is fired */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseCasing> CasingClass;

	/** Ammo eject's socket name (used for spawning the casing at this socket's location) */
	UPROPERTY(EditDefaultsOnly)
	FName AmmoEjectSocketName = FName("AmmoEject");

	/** Ability granted by this weapon */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> WeaponAbility;

	/** Base damage value */
	UPROPERTY(EditDefaultsOnly, meta = (UIMin = 0.f, ClampMin = 0.f))
	float BaseDamage;
	
	/** Damage gameplay effect */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffect;

	/** Fire rate (amount of shots performed in 1s) */
	UPROPERTY(EditDefaultsOnly, meta = (UIMin = 1.f, ClampMin = 1.f))
	float FireRate = 1.f;
	
	/** Weapon trace channel to apply damage */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "WeaponType == EWeaponType::Trace", EditConditionHides))
	TEnumAsByte<ETraceTypeQuery> WeaponTraceChannel = TraceTypeQuery3;
	
	/** Shooting distance */
	UPROPERTY(EditDefaultsOnly, meta = (UIMin = 0.f, ClampMin = 0.f, EditCondition = "WeaponType == EWeaponType::Trace", EditConditionHides, Units = "cm"))
	float ShootingDistance = 80000.f;

	/** Class of projectile to spawn */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "WeaponType == EWeaponType::Projectile", EditConditionHides))
	TSubclassOf<ABaseProjectile> ProjectileClass;

	/** Muzzle flash's socket name (used for spawning the projectile at this socket's location) */
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "WeaponType == EWeaponType::Projectile", EditConditionHides))
	FName MuzzleFlashSocketName = FName("MuzzleFlash");
};