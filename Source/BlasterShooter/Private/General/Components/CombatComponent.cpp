// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/CombatComponent.h"

// Unreal Engine
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// BlasterShooter
#include "Weapon/BaseWeapon.h"

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
	DOREPLIFETIME(UCombatComponent, bIsAiming);
}

#pragma endregion OVERRIDES

#pragma region EQUIPMENT

/** Equip weapon */
void UCombatComponent::EquipWeapon(ABaseWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}
	
	EquippedWeapon = Weapon;
	EquippedWeapon->SetWeaponState(EWeaponState::Equipped);

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (const USkeletalMeshSocket* WeaponSocket = Character->GetMesh()->GetSocketByName(WeaponSocketName))
		{
			WeaponSocket->AttachActor(EquippedWeapon, Character->GetMesh());
		}
		
		EquippedWeapon->SetOwner(Character);
		WeaponEquippedDelegate.Broadcast(true);
	}
}

/** RepNotify for EquippedWeapon */
void UCombatComponent::OnRep_EquippedWeapon()
{
	if (!EquippedWeapon)
	{
		return;
	}
	
	WeaponEquippedDelegate.Broadcast(true);
}

/** Fire weapon */
void UCombatComponent::FireWeapon()
{
	FHitResult TraceHitResult;
	TraceUnderCrosshair(TraceHitResult);
	ServerFireWeapon(TraceHitResult.ImpactPoint);
}

/** Server RPC for firing weapon */
void UCombatComponent::ServerFireWeapon_Implementation(const FVector_NetQuantize& HitTarget)
{
	MulticastFireWeapon(HitTarget);
}
	
/** Multicast RPC for firing weapon */
void UCombatComponent::MulticastFireWeapon_Implementation(const FVector_NetQuantize& HitTarget)
{
	if (!EquippedWeapon)
	{
		return;
	}
	
	EquippedWeapon->Fire(HitTarget);
}

/** Set whether this component's owner is aiming */
void UCombatComponent::SetIsAiming(bool bInIsAiming)
{
	bIsAiming = bInIsAiming;
	ServerSetIsAiming(bInIsAiming);
}

/** Server RPC for setting aiming */
void UCombatComponent::ServerSetIsAiming_Implementation(bool bInIsAiming)
{
	bIsAiming = bInIsAiming; 
}

/** Get EquippedWeapon's DataAsset */
UDataAsset_WeaponData* UCombatComponent::GetEquippedWeaponDataAsset() const
{
	return EquippedWeapon->GetWeaponDataAsset();
}

/** Perform trace under crosshair (from center of the screen) */
void UCombatComponent::TraceUnderCrosshair(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	const FVector2D CrosshairScreenLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairScreenLocation,
		CrosshairWorldLocation,
		CrosshairWorldDirection
	);

	if (!bScreenToWorld)
	{
		return;
	}
	
	if (const UDataAsset_WeaponData* WeaponDataAsset = EquippedWeapon->GetWeaponDataAsset())
	{
		const FWeaponData WeaponData = WeaponDataAsset->WeaponData;
		const FVector TraceStart = CrosshairWorldLocation;
		const FVector TraceEnd = TraceStart + CrosshairWorldDirection * WeaponData.ShootingDistance;
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(TArray<AActor*>{ GetOwner(), EquippedWeapon });

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			TraceStart,
			TraceEnd,
			UEngineTypes::ConvertToCollisionChannel(WeaponData.WeaponTraceChannel),
			QueryParams
		);

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = TraceEnd;
		}
	}
}

#pragma endregion EQUIPMENT
