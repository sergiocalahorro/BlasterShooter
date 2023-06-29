// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// BlasterShooter
#include "GAS/Abilities/BlasterAbility.h"

#include "Ability_Fire.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_PlayMontageAndWait;

// Forward declarations - BlasterShooter
class UCombatComponent;

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UAbility_Fire : public UBlasterAbility
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

	/** Start firing weapon */
	UFUNCTION()
	void StartFiring();

	/** Stop firing weapon */
	UFUNCTION()
	void StopFiring();

private:

	/** Avatar Actor's CombatComponent */
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent;

	/** Play montage's ability task */
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> PlayMontageTask;

	/** Timer handle for firing */
	UPROPERTY()
	FTimerHandle FireTimerHandle;

#pragma endregion ABILITY
	
};
