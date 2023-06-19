// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CombatComponent.generated.h"

// Forward declarations - BlasterShooter
class ABaseWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponEquippedSignature, bool, bShouldAffectMovement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUnequippedSignature, bool, bShouldAffectMovement);

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

	/** Equip weapon */
	UFUNCTION()
	void EquipWeapon(ABaseWeapon* Weapon);

	/** Returns whether this component's owner has an equipped weapon */
	UFUNCTION()
	bool IsWeaponEquipped() const { return EquippedWeapon != nullptr; }

	/** Returns whether this component's owner is aiming */
	UFUNCTION()
	bool IsAiming() const { return bIsAiming; }

	/** Set whether this component's owner is aiming */
	UFUNCTION()
	void SetIsAiming(bool bInIsAiming);

	/** Server RPC for setting aiming */
	UFUNCTION(Server, Reliable)
	void ServerSetIsAiming(bool bInIsAiming);

private:

	/** RepNotify for EquippedWeapon */
	UFUNCTION()
	void OnRep_EquippedWeapon();

public:

	/** Delegate called when weapon is equipped */
	UPROPERTY(BlueprintAssignable)
	FWeaponEquippedSignature WeaponEquippedDelegate;

	/** Delegate called when weapon is unequipped */
	UPROPERTY(BlueprintAssignable)
	FWeaponUnequippedSignature WeaponUnequippedDelegate;
	
protected:

	/** Socket name to attach the weapon to */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Equipment")
	FName WeaponSocketName = FName("RightHandSocket");

private:

	/** Currently equipped weapon */
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<ABaseWeapon> EquippedWeapon;
	
	/** Whether owner of this component is aiming */
	UPROPERTY(Replicated)
	bool bIsAiming;

#pragma endregion EQUIPMENT
	
};
 