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
	
protected:

	/** Socket name to attach the weapon to */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Equipment")
	FName WeaponSocketName;

private:

	/** Currently equipped weapon */
	UPROPERTY(Replicated)
	TObjectPtr<AWeaponActor> EquippedWeapon;

	/** Currently equipped weapon */
	UPROPERTY()
	TObjectPtr<ABlasterCharacter> BlasterCharacter;

#pragma endregion EQUIPMENT
	
};
 