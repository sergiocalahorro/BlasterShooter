// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

// BlasterShooter
#include "General/Enums/Animation/TurningInPlace.h"

#include "BlasterAnimInstance.generated.h"

// Forward declarations - BlasterShooter
class ABaseWeapon;
class ABlasterCharacter;

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

#pragma region OVERRIDES

public:

	/** Native initialization override point */
	virtual void NativeInitializeAnimation() override;

	/** Native update override point */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma endregion OVERRIDES

#pragma region CHARACTER

private:

	/** Set boolean variables */
	void SetBoolVariables(float DeltaSeconds);
	
	/** Set float variables */
	void SetFloatVariables(float DeltaSeconds);

	/** Set enum variables */
	void SetEnumVariables(float DeltaSeconds);

	/** Update weapon */
	void UpdateWeapon(float DeltaSeconds);

private:

	/** Character's reference */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	TObjectPtr<ABlasterCharacter> BlasterCharacter;

	/** Character's current speed */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	float Speed;

	/** Whether character's in air */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	/** Whether character's accelerating */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	bool bIsAccelerating;

	/** Whether character is crouched */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	bool bIsCrouched;

	/** Whether character has an equipped weapon */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	bool bIsWeaponEquipped;

	/** Currently equipped weapon */
	UPROPERTY()
	TObjectPtr<ABaseWeapon> EquippedWeapon;

	/** Left hand's socket name (in Weapon) */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	FName LeftHandSocketName = FName("LeftHandSocket");
	
	/** Whether character is aiming */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	bool bIsAiming;

	/** Yaw offset */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	float YawOffset;

	/** Character's delta rotation for calculating Yaw offset */
	FRotator DeltaYawOffsetRotation;

	/** Yaw offset's interpolation speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f))
	float YawOffsetInterpSpeed = 6.f;

	/** Lean */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	float Lean;

	/** Character's rotation on last frame */
	FRotator LastCharacterRotation;

	/** Character's current rotation */
	FRotator CurrentCharacterRotation;

	/** Lean's interpolation speed */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f))
	float LeanInterpSpeed = 3.f;

	/** AimOffset's Yaw */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	float AimOffsetYaw;

	/** AimOffset's Pitch */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	float AimOffsetPitch;

	/** Left hand's transform */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	FTransform LeftHandTransform;

	/** Character's turning in place state */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	ETurningInPlace TurningInPlace;

#pragma endregion CHARACTER
	
};
