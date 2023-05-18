// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

#include "CharacterData.generated.h"

// Forward declarations - Unreal Engine
class UGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	/** Character's effects */
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	/** Character's abilities */
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};