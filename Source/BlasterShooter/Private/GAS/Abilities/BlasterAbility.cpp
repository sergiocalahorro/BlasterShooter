// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/BlasterAbility.h"

#include "AbilitySystemComponent.h"

#pragma region OVERRIDES

/** Actually activate ability, do not call this directly */
void UBlasterAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ApplyEffects(OngoingEffectsAppliedOnStart);
	ApplyEffects(PermanentEffectsAppliedOnStart);
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
void UBlasterAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveEffects();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma endregion OVERRIDES

#pragma region ABILITY

/** Apply list of effects */
void UBlasterAbility::ApplyEffects(const TArray<TSubclassOf<UGameplayEffect>>& Effects, bool bArePermanent)
{
	if (Effects.IsEmpty())
	{
		return;
	}
	
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();

	const FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	for (const TSubclassOf<UGameplayEffect>& EffectClass : Effects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, EffectContext);
		if (!SpecHandle.IsValid())
		{
			continue;
		}

		FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (ActiveEffectHandle.WasSuccessfullyApplied() && !bArePermanent)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle);
		}
	}
}

/** Remove active effects */
void UBlasterAbility::RemoveEffects()
{
	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		for (const FActiveGameplayEffectHandle& ActiveEffect : ActiveEffectHandles)
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffect);
		}
	}
}

#pragma endregion ABILITY