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

	/** Whether character has an equipped weapon */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	bool bIsWeaponEquipped;

	/** Whether character is crouching */
	UPROPERTY(BlueprintReadOnly, Category = "AA|Character", meta = (AllowPrivateAccess = true))
	bool bIsCrouched; 

#pragma endregion CHARACTER
	
};
