// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "ProjectileData.generated.h"

// Forward declarations - Unreal Engine
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_USTRUCT_BODY()

public:
	
	/** Asset for this projectile's static mesh */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	/** Base damage value */
	UPROPERTY(EditDefaultsOnly)
	float BaseDamage;
	
	/** Damage radius */
	UPROPERTY(EditDefaultsOnly)
	float DamageRadius;
	
	/** Effects applied on this projectile's radial damage */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	/** Object types affected by this projectile's radial damage */
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> RadialDamageQueryTypes;

	/** Trace channel used on this projectile's radial damage */
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ETraceTypeQuery> RadialDamageTraceType;

	/** Initial speed */
	UPROPERTY(EditDefaultsOnly)
	float InitialSpeed = 3000.f;

	/** Maximum speed */
	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed = 3000.f;

	/** Gravity scale */
	UPROPERTY(EditDefaultsOnly)
	float GravityScale = 1.f;

	/** Whether initial velocity is set in local space */
	UPROPERTY(EditDefaultsOnly)
	bool bInitialVelocityInLocalSpace = false;

	/** Whether rotation follows velocity */
	UPROPERTY(EditDefaultsOnly)
	bool bRotationFollowsVelocity = true;

	/** Whether projectile should bounce */
	UPROPERTY(EditDefaultsOnly)
	bool bShouldBounce = false;
	
	/** Bounciness */
	UPROPERTY(EditDefaultsOnly, meta = (UIMin = 0.f, ClampMin = 0.f, EditCondition = "bShouldBounce", EditConditionHides))
	float Bounciness = 0.f;
};