// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"

// BlasterShooter
#include "GAS/Abilities/BlasterAbility.h"

#include "Ability_Aim.generated.h"

// Forward declarations - BlasterShooter
class UCameraModifier_Aim;

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UAbility_Aim : public UBlasterAbility
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

#pragma region AIM

protected:
	
	/** Aim camera modifier's class */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Aim")
	TSubclassOf<UCameraModifier> AimCameraModifierClass;

private:

	/** Aim camera modifier's reference */
	UPROPERTY()
	TObjectPtr<UCameraModifier_Aim> AimCameraModifier;

#pragma endregion AIM
	
};
