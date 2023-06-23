// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "HUDPackage.generated.h"

// Forward declarations - Unreal Engine
class UTexture2D;

USTRUCT(BlueprintType)
struct FHUDPackage
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

	/** Amount of spread for crosshairs */
	float CrosshairSpread;
};