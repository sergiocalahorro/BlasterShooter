﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/Combat/Ability_Fire.h"

// Unreal Engine
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

// BlasterShooter
#include "Character/BlasterCharacter.h"
#include "General/Components/CombatComponent.h"
#include "General/DataAssets/DataAsset_WeaponData.h"
#include "General/Structs/Data/WeaponData.h"

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UAbility_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ABlasterCharacter* BlasterCharacter = CastChecked<ABlasterCharacter>(GetAvatarActorFromActorInfo());
	UCombatComponent* CombatComponent = BlasterCharacter->GetCombatComponent();
	if (!CombatComponent)
	{
		return;
	}
	
	CombatComponent->FireWeapon();
	
	const FWeaponData WeaponData = CombatComponent->GetEquippedWeaponDataAsset()->WeaponData;
	if (UAnimMontage* AttackMontage = WeaponData.AttackMontage)
	{
		const FName MontageSection = BlasterCharacter->IsAiming() ? WeaponData.AttackMontageAimSectionName : WeaponData.AttackMontageHipSectionName;

		PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AttackMontage, 1.f, MontageSection);
		PlayMontageTask->OnCompleted.AddUniqueDynamic(this, &UAbility_Fire::K2_EndAbility);
		PlayMontageTask->OnCancelled.AddUniqueDynamic(this, &UAbility_Fire::K2_EndAbility);
		PlayMontageTask->OnInterrupted.AddUniqueDynamic(this, &UAbility_Fire::K2_EndAbility);
		PlayMontageTask->ReadyForActivation();
	}
}
	
/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UAbility_Fire::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsValid(PlayMontageTask))
	{
		PlayMontageTask->EndTask();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES