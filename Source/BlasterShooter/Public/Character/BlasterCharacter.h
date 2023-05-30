// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"

// BlasterShooter
#include "General/Structs/Data/CharacterData.h"

#include "BlasterCharacter.generated.h"

// Forward declarations - Unreal Engine
class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetComponent;

// Forward declarations - BlasterShooter
class UBlasterAttributeSet;
class UBlasterAbilitySystemComponent;
class UDataAsset_CharacterData;
class UOverheadWidget;
class UCombatComponent;
class AWeaponActor;

UCLASS()
class BLASTERSHOOTER_API ABlasterCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

#pragma region INITIALIZATON
	
public:
	
	/** Sets default values for this character's properties */
	ABlasterCharacter();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

public:

	/** Called every frame */
	virtual void Tick(float DeltaSeconds) override;
	
	/** Called upon landing when falling, to perform actions based on the Hit result */
	virtual void Landed(const FHitResult& Hit) override;

protected:

	/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
	virtual void PostInitializeComponents() override;

	/** Do any object-specific cleanup required immediately after loading an object. */
	virtual void PostLoad() override;
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when this Pawn is possessed (only called on the server) */
	virtual void PossessedBy(AController* NewController) override;

	/** PlayerState Replication Notification (only called on the client) */
	virtual void OnRep_PlayerState() override;

	/** Returns properties that are replicated for the lifetime of the actor channel */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Tell client that the Pawn is begin restarted */
	virtual void PawnClientRestart() override;

#pragma endregion OVERRIDES

#pragma region COMPONENTS

public:

	/** Getter of CameraBoom */
	UFUNCTION()
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Getter of FollowCamera */
	UFUNCTION()
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:

	/** Camera's spring arm */
	UPROPERTY(VisibleAnywhere, Category = "AA|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Camera */
	UPROPERTY(VisibleAnywhere, Category = "AA|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Component that will handle all combat-related functionality */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|Combat")
	TObjectPtr<UCombatComponent> CombatComponent;

	/** Overhead widget */
	UPROPERTY(EditAnywhere, Category = "AA|HUD")
	TObjectPtr<UWidgetComponent> OverheadWidget;

	/** Overhead widget reference */
	UPROPERTY()
	TObjectPtr<UOverheadWidget> OverheadWidgetRef;

#pragma endregion COMPONENTS

#pragma region INPUT

public:
	
	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	/** Called when movement input is triggered  */
	void InputAction_Move_Triggered(const FInputActionValue& Value);

	/** Called when looking input is triggered */
	void InputAction_Look_Triggered(const FInputActionValue& Value);

	/** Called when jump input is started */
	void InputAction_Jump_Started(const FInputActionValue& Value);

	/** Called when jump input is completed */
	void InputAction_Jump_Completed(const FInputActionValue& Value);

	/** Called when equip input is started */
	void InputAction_Equip_Started(const FInputActionValue& Value);

	/** Called when crouch input is started */
	void InputAction_Crouch_Started(const FInputActionValue& Value);

	/** Called when crouch input is completed */
	void InputAction_Crouch_Completed(const FInputActionValue& Value);

	/** Called when aiming input is started */
	void InputAction_Aim_Started(const FInputActionValue& Value);

	/** Called when aiming input is completed */
	void InputAction_Aim_Completed(const FInputActionValue& Value);

	/** RPC sent when equip input is started (client sends petition to server for equipping weapon) */
	UFUNCTION(Server, Reliable)
	void ServerInputAction_Equip_Started();

private:

	/** Default input mapping context */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** InputAction for movement input */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> InputAction_Move;

	/** InputAction for looking input */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> InputAction_Look;

	/** InputAction for jumping input */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> InputAction_Jump;

	/** InputAction for equipping input */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> InputAction_Equip;

	/** InputAction for crouching input */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> InputAction_Crouch;

	/** InputAction for aiming input */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Input")
	TObjectPtr<UInputAction> InputAction_Aim;

#pragma endregion INPUT

#pragma region CORE

public:

	/** Setter of CharacterData */
	UFUNCTION()
	void SetCharacterData(const FCharacterData& InCharacterData);

private:

	/** Initialize character */
	UFUNCTION()
	void InitializeCharacter();

protected:

	/** Data asset that holds character's data */
	UPROPERTY(EditAnywhere, Category = "AA|Core")
	TObjectPtr<UDataAsset_CharacterData> CharacterDataAsset;

private:

	/** Character's data */
	UPROPERTY()
	FCharacterData CharacterData;

#pragma endregion CORE

#pragma region WEAPON

public:

	/** Setter of OverlappingWeapon */
	UFUNCTION()
	void SetOverlappingWeapon(AWeaponActor* InOverlappingWeapon);
	
	/** Returns whether character has a weapon equipped */
	UFUNCTION()
	bool IsWeaponEquipped() const;

	/** Returns whether character is aiming */
	UFUNCTION()
	bool IsAiming() const;

private:

	/** RepNotify for OverlappingWeapon */
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeaponActor* OldOverlappingWeapon);

private:

	/** Overlapped weapon */
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeaponActor> OverlappingWeapon;

#pragma endregion WEAPON

#pragma region GAS

public:

	/** Getter of AbilitySystemComponent */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	/** Give abilities */
	UFUNCTION()
	void GiveAbilities() const;

	/** Apply gameplay effects at startup */
	UFUNCTION()
	void ApplyStartupEffects() const;

private:

	/** Function bound to the delegate that is called whenever the MaxMovementSpeed attribute is changed */
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);
	
private:

	/** Ability system component */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|GAS")
	TObjectPtr<UBlasterAbilitySystemComponent> AbilitySystemComponent;

	/** Attributes */
	UPROPERTY(VisibleDefaultsOnly, Category = "AA|GAS")
	TObjectPtr<UBlasterAttributeSet> AttributeSet;

#pragma region GAS_TAGS

	/** Jump tags */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Tags")
	FGameplayTagContainer JumpTags;

	/** Crouch tags */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Tags")
	FGameplayTagContainer CrouchTags;
	
	/** Aim tags */
	UPROPERTY(EditDefaultsOnly, Category = "AA|GAS|Tags")
	FGameplayTagContainer AimTags;
	
#pragma endregion GAS_TAGS

#pragma endregion GAS
	
};
