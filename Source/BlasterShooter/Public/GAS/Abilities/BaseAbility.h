// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "BaseAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UBaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion OVERRIDES

#pragma region ABILITY

private:

	/** Apply list of effects */
	UFUNCTION()
	void ApplyEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects, bool bArePermanent = false);

	/** Remove active effects */
	UFUNCTION()
	void RemoveEffects();

protected:

	/** List of effects to apply when ability is activated */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Ability")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsAppliedOnStart;

	/** List of effects to remove when ability is ended */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Ability")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsRemovedOnEnd;

	/** List of effects to apply permanently when ability is activated */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Ability")
	TArray<TSubclassOf<UGameplayEffect>> PermanentEffectsAppliedOnStart;

private:

	/** List of active effects */
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

#pragma endregion ABILITY
	
};
