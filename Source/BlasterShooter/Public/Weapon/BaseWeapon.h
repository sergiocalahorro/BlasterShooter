﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// BlasterShooter
#include "General/DataAssets/DataAsset_WeaponData.h"
#include "General/Enums/Weapon/WeaponState.h"

#include "BaseWeapon.generated.h"

// Forward declarations - Unreal Engine
class USphereComponent;
class UWidgetComponent;
class UGameplayAbility;

// Forward declarations - BlasterShooter
class UDataAsset_WeaponData;
class UDataAsset_CasingData;

UCLASS()
class BLASTERSHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

#pragma region INITIALIZATION
	
public:
	
	/** Sets default values for this actor's properties */
	ABaseWeapon();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

protected:
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when an instance of this class is placed (in editor) or spawned */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

protected:
		
	/** Weapon's mesh */
	UPROPERTY()
	TObjectPtr<UMeshComponent> WeaponMesh;
	
	/** Pick up sphere */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Weapon")
	TObjectPtr<USphereComponent> PickupTrigger;

	/** Pick Up widget */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Weapon")
	TObjectPtr<UWidgetComponent> PickupWidget;

#pragma endregion COMPONENTS

#pragma region WEAPON

public:

	/** Fire weapon */
	UFUNCTION()
	virtual void Fire(const FVector& HitTarget);

	/** Show/hide pickup widget */
	UFUNCTION()
	void TogglePickupWidget(bool bShowWidget) const;

	/** Getter of WeaponState */
	UFUNCTION()
	EWeaponState GetWeaponState() const { return WeaponState; }

	/** Setter of WeaponState */
	UFUNCTION()
	void SetWeaponState(EWeaponState InWeaponState);
	
	/** Getter of WeaponDataAsset */
	UFUNCTION()
	UDataAsset_WeaponData* GetWeaponDataAsset() const { return WeaponDataAsset; }
	
	/** Getter of WeaponData */
	UFUNCTION()
	FWeaponData GetWeaponData() const { return WeaponDataAsset->WeaponData; }

	/** Get Weapon's skeletal mesh */
	UFUNCTION()
	USkeletalMeshComponent* GetWeaponSkeletalMesh() const { return CastChecked<USkeletalMeshComponent>(WeaponMesh); }

	/** Getter of Weapon's ability */
	UFUNCTION()
	TSubclassOf<UGameplayAbility> GetWeaponAbility() const;

protected:

	/** Initialize */
	UFUNCTION()
	void Initialize();

	/** Function bound to pickup trigger's begin overlap event */
	UFUNCTION()
	void OnPickupTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Function bound to pickup trigger's end overlap event */
	UFUNCTION()
	void OnPickupTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:

	/** RepNotify for WeaponState */
	UFUNCTION()
	void OnRep_WeaponState();
	
protected:

	/** Current weapon's state */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_WeaponState, Category = "AA|Weapon")
	EWeaponState WeaponState = EWeaponState::None;

	/** Weapon's data asset */
	UPROPERTY(EditAnywhere, Category = "AA|Weapon")
	TObjectPtr<UDataAsset_WeaponData> WeaponDataAsset;

	/** Casing's data asset */
	UPROPERTY(EditAnywhere, Category = "AA|Weapon")
	TObjectPtr<UDataAsset_CasingData> CasingDataAsset;

#pragma endregion WEAPON

};
