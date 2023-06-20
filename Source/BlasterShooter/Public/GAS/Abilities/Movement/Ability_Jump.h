// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// BlasterShooter
#include "GAS/Abilities/BlasterAbility.h"

#include "Ability_Jump.generated.h"

// Forward declarations - Unreal Engine
class UAbilityTask_WaitInputRelease;

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UAbility_Jump : public UBlasterAbility
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:

	/** Returns true if this ability can be activated right now. Has no side effects */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

#pragma endregion OVERRIDES
	
#pragma region JUMP

private:

	/** Function bound to async task's input released delegate */
	UFUNCTION()
	void OnJumpInputReleased(float TimeHeld);

	/** Function bound to character's on landed delegate */
	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

private:

	/** Async task to wait for input release */
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputRelease> TaskWaitInputRelease;

#pragma endregion JUMP
	
};
