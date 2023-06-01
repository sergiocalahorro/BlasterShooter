// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "TurningInPlace.generated.h"

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	NotTurning,
	Left,
	Right,
};