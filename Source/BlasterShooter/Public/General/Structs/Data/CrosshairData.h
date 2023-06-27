// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "CrosshairData.generated.h"

// Forward declarations - Unreal Engine
class UTexture2D;

USTRUCT(BlueprintType)
struct FCrosshairData
{
	GENERATED_USTRUCT_BODY()

public:

	/** Texture for center crosshairs */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CrosshairsCenter;

	/** Texture for left crosshairs */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CrosshairsLeft;

	/** Texture for right crosshairs */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CrosshairsRight;

	/** Texture for top crosshairs */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CrosshairsTop;

	/** Texture for bottom crosshairs */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CrosshairsBottom;

	/** Base spread value */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float BaseSpread = 0.5f;

	/** Crosshair's min spread in air factor */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinSpreadInAirFactor = 0.f;

	/** Crosshair's max spread in air factor */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxSpreadInAirFactor = 2.25f;

	/** Crosshair's spread min interp speed in air */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float SpreadInAirMinInterpSpeed = 2.25f;
	
	/** Crosshair's spread max interp speed in air */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float SpreadInAirMaxInterpSpeed = 30.f;

	/** Crosshair's min spread aiming factor */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinSpreadAimFactor = 0.f;

	/** Crosshair's max spread aiming factor */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MaxSpreadAimFactor = 0.6f;

	/** Crosshair's spread min interp speed aiming */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float SpreadAimMinInterpSpeed = 30.f;
	
	/** Crosshair's spread max interp speed aiming */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float SpreadAimMaxInterpSpeed = 30.f;

	/** Crosshair's spread firing factor */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float SpreadFireFactor = 0.75f;

	/** Crosshair's min spread firing factor */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float MinSpreadFireFactor = 0.f;

	/** Crosshair's spread min interp speed firing */
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float SpreadFireMinInterpSpeed = 40.f;
};