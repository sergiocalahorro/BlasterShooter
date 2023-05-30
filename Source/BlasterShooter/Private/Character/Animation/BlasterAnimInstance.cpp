// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Animation/BlasterAnimInstance.h"

// Unreal Engine
#include "GameFramework/CharacterMovementComponent.h"

// BlasterShooter
#include "Character/BlasterCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region OVERRIDES

/** Native initialization override point */
void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Set character's reference
	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

/** Native update override point */
void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Set character's reference in the first frame if it's not yet initialized
	if (!BlasterCharacter)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
		if (!BlasterCharacter)
		{
			return;
		}
	}

	SetBoolVariables(DeltaSeconds);
	SetFloatVariables(DeltaSeconds);
}

#pragma endregion OVERRIDES

#pragma region CHARACTER

/** Set boolean variables */
void UBlasterAnimInstance::SetBoolVariables(float DeltaSeconds)
{
	// Set whether character's in air
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	// Set whether character accelerating
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

	// Set whether character is crouched
	bIsCrouched = BlasterCharacter->bIsCrouched;
	
	// Set whether character has a weapon equipped
	bIsWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

	// Set whether character is aiming
	bIsAiming = BlasterCharacter->IsAiming();
}

/** Set float variables */
void UBlasterAnimInstance::SetFloatVariables(float DeltaSeconds)
{
	// Calculate character's current speed
	const FVector Velocity = BlasterCharacter->GetVelocity();
	Speed = Velocity.Size2D();

	// Calculate Yaw offset for strafing
	const FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());
	const FRotator TargetDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaYawOffsetRotation = FMath::RInterpTo(DeltaYawOffsetRotation, TargetDeltaRotation, DeltaSeconds, YawOffsetInterpSpeed);
	YawOffset = DeltaYawOffsetRotation.Yaw;

	// Calculate character's leaning
	LastCharacterRotation = CurrentCharacterRotation;
	CurrentCharacterRotation = BlasterCharacter->GetActorRotation();
	const FRotator DeltaCharacterRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentCharacterRotation, LastCharacterRotation);
	const float TargetLean = DeltaCharacterRotation.Yaw / DeltaSeconds;
	const float InterpLean = FMath::FInterpTo(Lean, TargetLean, DeltaSeconds, LeanInterpSpeed);
	Lean = FMath::Clamp(InterpLean, -90.f, 90.f);
}

#pragma endregion CHARACTER