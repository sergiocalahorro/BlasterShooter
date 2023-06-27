// Fill out your copyright notice in the Description page of Project Settings.

#include "General/Components/CombatComponent.h"

// Unreal Engine
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// BlasterShooter
#include "Character/BlasterCharacter.h"
#include "General/Structs/HUD/HUDPackage.h"
#include "Weapon/BaseWeapon.h"

#pragma region INITIALIZATION

/** Sets default values for this component's properties */
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the game starts */
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	BlasterCharacter = Cast<ABlasterCharacter>(GetOwner());
}

/** Called every frame */
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateHUDCrossHairs(DeltaTime);

	if (BlasterCharacter && BlasterCharacter->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshair(HitResult);
		HitTarget = HitResult.ImpactPoint;
	}
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
	WeaponStartedFireDelegate.Broadcast();

	FHitResult TraceHitResult;
	TraceUnderCrosshair(TraceHitResult);
	ServerFireWeapon(TraceHitResult.ImpactPoint);
}

/** Server RPC for firing weapon */
void UCombatComponent::ServerFireWeapon_Implementation(const FVector_NetQuantize& NewHitTarget)
{
	MulticastFireWeapon(NewHitTarget);
}
	
/** Multicast RPC for firing weapon */
void UCombatComponent::MulticastFireWeapon_Implementation(const FVector_NetQuantize& NewHitTarget)
{
	if (!EquippedWeapon)
	{
		return;
	}

	CrosshairFireFactor = EquippedWeapon->GetWeaponData().CrosshairData.SpreadFireFactor;
	EquippedWeapon->Fire(NewHitTarget);
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

/** Get EquippedWeapon's data */
FWeaponData UCombatComponent::GetEquippedWeaponData() const
{
	return EquippedWeapon->GetWeaponData();
}

/** Perform trace under crosshair (from center of the screen) */
void UCombatComponent::TraceUnderCrosshair(FHitResult& TraceHitResult)
{
	if (!EquippedWeapon)
	{
		return;
	}
	
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
		FVector TraceStart = CrosshairWorldLocation;
		if (BlasterCharacter)
		{
			const float DistanceToCharacter = (BlasterCharacter->GetActorLocation() - TraceStart).Size();
			TraceStart += CrosshairWorldDirection * (DistanceToCharacter + TraceOffset);
		}
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

		const bool bReactToCrosshair = TraceHitResult.GetActor() && Cast<IReactToCrosshair>(TraceHitResult.GetActor());
		CrosshairsTraceHitDelegate.Broadcast(bReactToCrosshair);

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = TraceEnd;
		}
	}
}

/** Update HUD crosshairs */
void UCombatComponent::UpdateHUDCrossHairs(float DeltaTime)
{
	FHUDPackage HUDPackage;
	
	if (EquippedWeapon)
	{
		const FWeaponData WeaponData = EquippedWeapon->GetWeaponData();
		HUDPackage.CrosshairsCenter = WeaponData.CrosshairData.CrosshairsCenter;
		HUDPackage.CrosshairsLeft = WeaponData.CrosshairData.CrosshairsLeft;
		HUDPackage.CrosshairsRight = WeaponData.CrosshairData.CrosshairsRight;
		HUDPackage.CrosshairsTop = WeaponData.CrosshairData.CrosshairsTop;
		HUDPackage.CrosshairsBottom = WeaponData.CrosshairData.CrosshairsBottom;

		// Calculate crosshair spread
		if (BlasterCharacter)
		{
			const FVector2D WalkSpeedRange(0.f, BlasterCharacter->GetCharacterMovement()->MaxWalkSpeed);
			const FVector2D VelocityMultiplierRange(0.f, 1.f);
			const FVector Velocity = BlasterCharacter->GetVelocity();
			const float CrosshairSpreadVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size2D());

			// In air factor
			if (BlasterCharacter->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, WeaponData.CrosshairData.MaxSpreadInAirFactor, DeltaTime, WeaponData.CrosshairData.SpreadInAirMinInterpSpeed);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, WeaponData.CrosshairData.MinSpreadInAirFactor, DeltaTime, WeaponData.CrosshairData.SpreadInAirMaxInterpSpeed);
			}

			// Aim factor
			if (bIsAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, WeaponData.CrosshairData.MaxSpreadAimFactor, DeltaTime, WeaponData.CrosshairData.SpreadAimMinInterpSpeed);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, WeaponData.CrosshairData.MinSpreadAimFactor, DeltaTime, WeaponData.CrosshairData.SpreadAimMaxInterpSpeed);
			}

			// Firing factor
			CrosshairFireFactor = FMath::FInterpTo(CrosshairFireFactor, WeaponData.CrosshairData.MinSpreadFireFactor, DeltaTime, WeaponData.CrosshairData.SpreadFireMinInterpSpeed);

			HUDPackage.CrosshairSpread = WeaponData.CrosshairData.BaseSpread +
										 CrosshairSpreadVelocityFactor +
										 CrosshairInAirFactor -
										 CrosshairAimFactor +
										 CrosshairFireFactor;
		}
	}
	
	CrosshairsUpdateDelegate.Broadcast(HUDPackage);
}

#pragma endregion EQUIPMENT
