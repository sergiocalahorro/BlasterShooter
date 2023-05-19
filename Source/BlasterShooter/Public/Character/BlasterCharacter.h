// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "General/Structs/Data/CharacterData.h"

#include "BlasterCharacter.generated.h"

class UDataAsset_CharacterData;
// Forward declarations - Unreal Engine
class UInputComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

// Forward declarations - BlasterShooter
class UBlasterAttributeSet;
class UBlasterAbilitySystemComponent;

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

#pragma endregion COMPONENTS

#pragma region INPUT

public:
	
	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called when jump is started */
	void StartJump(const FInputActionValue& Value);

	/** Called when jump is stopped */
	void StopJump(const FInputActionValue& Value);

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

#pragma endregion INPUT

#pragma region CORE

public:

	/** Setter of CharacterData */
	UFUNCTION()
	void SetCharacterData(const FCharacterData& InCharacterData);

protected:

	/** Data asset that holds character's data */
	UPROPERTY(EditAnywhere, Category = "AA|Core")
	TObjectPtr<UDataAsset_CharacterData> CharacterDataAsset;

private:

	/** Character's data */
	UPROPERTY()
	FCharacterData CharacterData;

#pragma endregion CORE

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
	
#pragma endregion GAS_TAGS

#pragma endregion GAS
	
};
