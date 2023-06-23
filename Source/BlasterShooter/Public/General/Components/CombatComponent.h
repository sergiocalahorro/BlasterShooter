﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// BlasterShooter
#include "General/Structs/HUD/HUDPackage.h"

#include "CombatComponent.generated.h"

// Forward declarations - BlasterShooter
class UDataAsset_WeaponData;
class ABaseWeapon;
class ABlasterCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquippedSignature, bool, bShouldAffectMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUnequippedSignature, bool, bShouldAffectMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrosshairsUpdateSignature, const FHUDPackage&, HUDPackage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLASTERSHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:
	
	/** Sets default values for this component's properties */
	UCombatComponent();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:
	
	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region EQUIPMENT

public:

	/** Getter of EquippedWeapon */
	UFUNCTION()
	ABaseWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	/** Get EquippedWeapon's DataAsset */
	UFUNCTION()
	UDataAsset_WeaponData* GetEquippedWeaponDataAsset() const;

	/** Equip weapon */
	UFUNCTION()
	void EquipWeapon(ABaseWeapon* Weapon);

	/** Fire weapon */
	UFUNCTION()
	void FireWeapon();

	/** Returns whether this component's owner has an equipped weapon */
	UFUNCTION()
	bool IsWeaponEquipped() const { return EquippedWeapon != nullptr; }

	/** Returns whether this component's owner is aiming */
	UFUNCTION()
	bool IsAiming() const { return bIsAiming; }

	/** Set whether this component's owner is aiming */
	UFUNCTION()
	void SetIsAiming(bool bInIsAiming);
	
protected:

	/** Server RPC for setting aiming */
	UFUNCTION(Server, Reliable)
	void ServerSetIsAiming(bool bInIsAiming);

	/** Perform trace under crosshair (from center of the screen) */
	void TraceUnderCrosshair(FHitResult& TraceHitResult);

private:

	/** RepNotify for EquippedWeapon */
	UFUNCTION()
	void OnRep_EquippedWeapon();

	/** Server RPC for firing weapon */
	UFUNCTION(Server, Reliable)
	void ServerFireWeapon(const FVector_NetQuantize& HitTarget);
	
	/** Multicast RPC for firing weapon */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireWeapon(const FVector_NetQuantize& HitTarget);

public:

	/** Delegate called when weapon is equipped */
	FWeaponEquippedSignature WeaponEquippedDelegate;

	/** Delegate called when weapon is unequipped */
	FWeaponUnequippedSignature WeaponUnequippedDelegate;
	
protected:

	/** Socket name to attach the weapon to */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Equipment")
	FName WeaponSocketName = FName("RightHandSocket");

private:

	/** Currently equipped weapon */
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<ABaseWeapon> EquippedWeapon;

	/** Character's reference */
	UPROPERTY()
	TObjectPtr<ABlasterCharacter> BlasterCharacter;
	
	/** Whether owner of this component is aiming */
	UPROPERTY(Replicated)
	bool bIsAiming;
	
#pragma endregion EQUIPMENT

#pragma region HUD

private:
	
	/** Update HUD crosshairs */
	void UpdateHUDCrossHairs(float DeltaTime);

public:
	
	/** Delegate called when crosshairs are updated */
	FCrosshairsUpdateSignature CrosshairsUpdateDelegate;

private:

	/** Crosshair's min spread in air factor */
	UPROPERTY(EditDefaultsOnly, Category = "AA|HUD", meta = (ClampMin = 0.f, UIMin = 0.f))
	float CrosshairMinSpreadInAirFactor = 0.f;

	/** Crosshair's max spread in air factor */
	UPROPERTY(EditDefaultsOnly, Category = "AA|HUD", meta = (ClampMin = 0.f, UIMin = 0.f))
	float CrosshairMaxSpreadInAirFactor = 2.25f;

	/** Crosshair's spread in air min interp speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|HUD", meta = (ClampMin = 0.f, UIMin = 0.f))
	float CrosshairSpreadInAirMinSpeed = 2.25f;
	
	/** Crosshair's spread in air max interp speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|HUD", meta = (ClampMin = 0.f, UIMin = 0.f))
	float CrosshairSpreadInAirMaxSpeed = 30.f;
	
	/** Crosshair's current spread in air factor */
	float CrosshairSpreadInAirFactor;

#pragma endregion HUD
	
};
 