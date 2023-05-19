// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Animation/BlasterAnimInstance.h"

// Unreal Engine
#include "GameFramework/CharacterMovementComponent.h"

// BlasterShooter
#include "Character/BlasterCharacter.h"

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
	
	// Calculate character's current speed
	const FVector Velocity = BlasterCharacter->GetVelocity();
	Speed = Velocity.Size2D();

	// Set whether character's in air
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	// Set whether character accelerating
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
}

#pragma endregion OVERRIDES