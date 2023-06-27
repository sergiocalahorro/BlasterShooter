// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/Modifiers/CameraModifier_Aim.h"

#pragma region OVERRIDES

/** Directly modifies variables in the owning camera */
bool UCameraModifier_Aim::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
	CurrentAimFOV = FMath::FInterpTo(CurrentAimFOV, AimFOV, DeltaTime, AimInterpSpeed);
	InOutPOV.FOV = CurrentAimFOV;
	return true;
}

#pragma endregion OVERRIDES