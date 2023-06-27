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

public:

	/** Setter of AimFOV */
	void SetAimFOV(float InAimFOV) { AimFOV = InAimFOV; }

	/** Setter of AimInterpSpeed */
	void SetAimInterpSpeed(float InAimInterpSpeed) { AimInterpSpeed = InAimInterpSpeed; }

private:

	/** FOV for zoom while aiming */
	float CurrentAimFOV = 90.f;

	/** FOV for zoom while aiming */
	float AimFOV;

	/** Interp speed to zoom while aiming */
	float AimInterpSpeed;

#pragma endregion AIM
	
};
