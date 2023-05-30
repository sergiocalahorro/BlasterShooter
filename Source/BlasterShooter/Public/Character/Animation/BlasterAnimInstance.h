// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "BlasterAnimInstance.generated.h"

// Forward declarations - BlasterShooter
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

#pragma endregion CHARACTER
	
};
