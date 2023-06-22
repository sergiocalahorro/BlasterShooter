// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "CasingData.generated.h"

// Forward declarations - Unreal Engine
class USoundCue;

USTRUCT(BlueprintType)
struct FCasingData
{
	GENERATED_USTRUCT_BODY()

public:
	
	/** Asset for this casing's static mesh */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	/** Impulse to eject casing */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float EjectionImpulse = 10.f;

	/** Sound played when the casing hits the ground */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundCue> HitSound;

	/** Time to destroy the casing after it hit the ground */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f, ForceUnits = "s"))
	float Lifespan;

	/** Min angle offset to spawn */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = -45.f, UIMin = -45.f, ClampMax = 45.f, UIMax = 45.f))
	float MinSpawnAngleOffset = -45.f;

	/** Max angle offset to spawn */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = -45.f, UIMin = -45.f, ClampMax = 45.f, UIMax = 45.f))
	float MaxSpawnAngleOffset = 45.f;
};