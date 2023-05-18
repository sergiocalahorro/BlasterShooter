// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AbilitySystem/BlasterAbilitySystemComponent.h"

#pragma region ABILITIES

/** Give incoming abilities to owner */
void UBlasterAbilitySystemComponent::GiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : InAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(Ability, 1.f, INDEX_NONE, nullptr));
	}
}

#pragma endregion ABILITIES

#pragma region EFFECTS
	
/** Apply incoming effects to owner */
void UBlasterAbilitySystemComponent::ApplyEffects(const TArray<TSubclassOf<UGameplayEffect>>& InEffects)
{
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(GetOwner());
		
	for (const TSubclassOf<UGameplayEffect> Effect : InEffects)
	{
		if (Effect.Get())
		{
			const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Effect, 1.f, EffectContext);
			if (SpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

#pragma endregion EFFECTS