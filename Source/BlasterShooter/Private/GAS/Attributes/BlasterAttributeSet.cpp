// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Attributes/BlasterAttributeSet.h"

// Unreal Engine
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#pragma region OVERRIDES

/**	Called just before a GameplayEffect is executed to modify the base value of an attribute. No more changes can be made. */
void UBlasterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxMovementSpeedAttribute())
	{
		if (const ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor()))
		{
			if (UCharacterMovementComponent* CharacterMovement = OwningCharacter->GetCharacterMovement())
			{
				CharacterMovement->MaxWalkSpeed = GetMaxMovementSpeed();
			}
		}
	}
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void UBlasterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBlasterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBlasterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBlasterAttributeSet, MaxMovementSpeed, COND_None, REPNOTIFY_Always);
}

#pragma endregion OVERRIDES

#pragma region CORE

/** Replicate Health */
void UBlasterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlasterAttributeSet, Health, OldHealth);
}

/** Replicate MaxHealth */
void UBlasterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlasterAttributeSet, MaxHealth, OldMaxHealth);
}

/** Replicate MaxMovementSpeed */
void UBlasterAttributeSet::OnRep_MaxMovementSpeed(const FGameplayAttributeData& OldMaxMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlasterAttributeSet, MaxMovementSpeed, OldMaxMovementSpeed);
}

#pragma endregion CORE