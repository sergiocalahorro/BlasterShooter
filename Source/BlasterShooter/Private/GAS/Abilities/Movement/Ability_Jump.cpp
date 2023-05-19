// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Movement/Ability_Jump.h"

// Unreal Engine
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#pragma region OVERRIDES

/** Returns true if this ability can be activated right now. Has no side effects */
bool UAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	return Character->CanJump();
}

/** Actually activate ability, do not call this directly */
void UAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->Jump();
	Character->LandedDelegate.AddUniqueDynamic(this, &UAbility_Jump::StopJump);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	StopJump(FHitResult());
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES

#pragma region JUMP

/** Stop character's jump */
void UAbility_Jump::StopJump(const FHitResult& Hit)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character->GetCharacterMovement()->IsFalling())
	{
		Character->StopJumping();
		Character->LandedDelegate.RemoveDynamic(this, &UAbility_Jump::StopJump);
	}
}

#pragma endregion JUMP