// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CameraModifier_Aim.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UCameraModifier_Aim : public UCameraModifier
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:

	/** Directly modifies variables in the owning camera */
	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;

#pragma endregion OVERRIDES

#pragma region AIM

protected:

	/** Distance to move the view location to */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Aim", meta = (ClampMin = 0.f, UIMIn = 0.f))
	float AimDistance = 150.f;

#pragma endregion AIM
	
};
