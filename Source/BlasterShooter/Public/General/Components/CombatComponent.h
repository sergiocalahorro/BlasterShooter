// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// BlasterShooter
#include "General/Structs/Data/WeaponData.h"
#include "General/Structs/HUD/HUDPackage.h"

#include "CombatComponent.generated.h"

// Forward declarations - BlasterShooter
class UDataAsset_WeaponData;
class ABaseWeapon;
class ABlasterCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquippedSignature, bool, bShouldAffectMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUnequippedSignature, bool, bShouldAffectMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponStartedFireSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponStoppedFireSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrosshairsTraceHitSignature, bool, bReactToCrosshair);
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

	/** Get EquippedWeapon's data */
	UFUNCTION()
	FWeaponData GetEquippedWeaponData() const;

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
	void ServerFireWeapon(const FVector_NetQuantize& NewHitTarget);
	
	/** Multicast RPC for firing weapon */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireWeapon(const FVector_NetQuantize& NewHitTarget);

public:

	/** Delegate called when weapon is equipped */
	FWeaponEquippedSignature WeaponEquippedDelegate;

	/** Delegate called when weapon is unequipped */
	FWeaponUnequippedSignature WeaponUnequippedDelegate;

	/** Delegate called when weapon started firing */
	FWeaponStartedFireSignature WeaponStartedFireDelegate;

	/** Delegate called when weapon stopped firing */
	FWeaponStoppedFireSignature WeaponStoppedFireDelegate;
	
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

	/** Distance offset used for trace under crosshairs */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Equipment", meta = (ClampMin = 0.f, UIMin = 0.f))
	float TraceOffset = 100.f;
	
#pragma endregion EQUIPMENT

#pragma region HUD

private:
	
	/** Update HUD crosshairs */
	void UpdateHUDCrossHairs(float DeltaTime);

public:
	
	/** Delegate called when crosshairs are updated */
	FCrosshairsUpdateSignature CrosshairsUpdateDelegate;

	/** Delegate called when trace hits an Actor that reacts to crosshairs */
	FCrosshairsTraceHitSignature CrosshairsTraceHitDelegate;

	/** Hit target (location it's aiming at) */
	FVector HitTarget;

private:

	/** Crosshair's spread factor when in air */
	float CrosshairInAirFactor;

	/** Crosshair's spread factor when aiming */
	float CrosshairAimFactor;

	/** Crosshair's spread factor when firing */
	float CrosshairFireFactor;

#pragma endregion HUD
	
}; 
 