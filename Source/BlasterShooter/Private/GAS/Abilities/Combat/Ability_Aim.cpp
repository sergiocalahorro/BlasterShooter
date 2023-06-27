// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Combat/Ability_Aim.h"

// BlasterShooter
#include "Camera/Modifiers/CameraModifier_Aim.h"
#include "General/Components/CombatComponent.h"

#pragma region OVERRIDES

/** Returns true if this ability can be activated right now. Has no side effects */
bool UAbility_Aim::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const UCombatComponent* CombatComponent = Cast<UCombatComponent>(ActorInfo->AvatarActor.Get()->FindComponentByClass(UCombatComponent::StaticClass())))
	{
		return CombatComponent->IsWeaponEquipped();
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

/** Actually activate ability, do not call this directly */
void UAbility_Aim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCombatComponent* CombatComponent = CastChecked<UCombatComponent>(ActorInfo->AvatarActor.Get()->FindComponentByClass(UCombatComponent::StaticClass()));
	CombatComponent->SetIsAiming(true);

	if (APlayerCameraManager* PlayerCameraManager = ActorInfo->PlayerController->PlayerCameraManager)
	{
		AimCameraModifier = CastChecked<UCameraModifier_Aim>(PlayerCameraManager->AddNewCameraModifier(AimCameraModifierClass));
		AimCameraModifier->SetAimFOV(CombatComponent->GetEquippedWeaponData().AimFOV);
		AimCameraModifier->SetAimInterpSpeed(CombatComponent->GetEquippedWeaponData().AimInterpSpeed);
	}
	else
	{
		K2_EndAbility();
	}
}
	
/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_Aim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UCombatComponent* CombatComponent = Cast<UCombatComponent>(ActorInfo->AvatarActor.Get()->FindComponentByClass(UCombatComponent::StaticClass())))
	{
		CombatComponent->SetIsAiming(false);
	}

	if (const APlayerController* PlayerController = ActorInfo->PlayerController.Get())
	{
		if (APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager)
		{
			if (AimCameraModifier)
			{
				PlayerCameraManager->RemoveCameraModifier(AimCameraModifier);
			}
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES
