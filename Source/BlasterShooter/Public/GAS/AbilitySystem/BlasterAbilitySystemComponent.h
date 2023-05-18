// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "BlasterAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLASTERSHOOTER_API UBlasterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

#pragma region ABILITIES

public:

	/** Give incoming abilities to owner */
	UFUNCTION()
	void GiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities);

#pragma endregion ABILITIES

#pragma region EFFECTS
	
public:

	/** Apply incoming effects to owner */
	UFUNCTION()
	void ApplyEffects(const TArray<TSubclassOf<UGameplayEffect>>& InEffects);

#pragma endregion EFFECTS
	
};
