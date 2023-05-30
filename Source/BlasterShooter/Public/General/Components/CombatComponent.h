// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CombatComponent.generated.h"

// Forward declarations - BlasterShooter
class ABlasterCharacter;
class AWeaponActor;

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

	/** Setter of BlasterCharacter */
	UFUNCTION()
	void SetBlasterCharacter(ABlasterCharacter* InBlasterCharacter) { BlasterCharacter = InBlasterCharacter; }

	/** Equip weapon */
	UFUNCTION()
	void EquipWeapon(AWeaponActor* Weapon);

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
	
protected:

	/** Socket name to attach the weapon to */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Equipment")
	FName WeaponSocketName;

private:

	/** Character's reference */
	UPROPERTY()
	TObjectPtr<ABlasterCharacter> BlasterCharacter;

	/** Currently equipped weapon */
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<AWeaponActor> EquippedWeapon;
	
	/** Whether owner of this component is aiming */
	UPROPERTY(Replicated)
	bool bIsAiming;

#pragma endregion EQUIPMENT
	
};
 