// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "WeaponState.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	None,
	Equipped,
	Dropped
};