// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/CombatComponent.h"

// Unreal Engine
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

// BlasterShooter
#include "Character/BlasterCharacter.h"
#include "Weapon/WeaponActor.h"

#pragma region INITIALIZATION

/** Sets default values for this component's properties */
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the game starts */
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

/** Called every frame */
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
}

#pragma endregion OVERRIDES

#pragma region EQUIPMENT

/** Equip weapon */
void UCombatComponent::EquipWeapon(AWeaponActor* Weapon)
{
	if (!BlasterCharacter || !Weapon)
	{
		return;
	}
	
	EquippedWeapon = Weapon;
	EquippedWeapon->SetWeaponState(EWeaponState::Equipped);

	if (const USkeletalMeshSocket* WeaponSocket = BlasterCharacter->GetMesh()->GetSocketByName(WeaponSocketName))
	{
		WeaponSocket->AttachActor(EquippedWeapon, BlasterCharacter->GetMesh());
	}
	
	EquippedWeapon->SetOwner(BlasterCharacter);
}

#pragma endregion EQUIPMENT
