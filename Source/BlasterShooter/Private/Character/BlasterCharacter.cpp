﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BlasterCharacter.h"

// Unreal Engine
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// BlasterShooter
#include "BlasterShooter.h"
#include "General/DataAssets/DataAsset_CharacterData.h"
#include "GAS/AbilitySystem/BlasterAbilitySystemComponent.h"
#include "GAS/Attributes/BlasterAttributeSet.h"
#include "General/Components/CombatComponent.h"
#include "UI/HUD/OverheadWidget.h"
#include "Weapon/BaseWeapon.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup hierarchy
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	CameraBoom->TargetArmLength = 450.f;
	CameraBoom->SocketOffset = FVector(0.f, 75.f, 75.f);
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	// Configure movement
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 900.f, 0.f);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->JumpZVelocity = 1600.f;
	GetCharacterMovement()->GravityScale = 4.f;

	// Configure collisions
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	// GAS setup
	AbilitySystemComponent = CreateDefaultSubobject<UBlasterAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UBlasterAttributeSet>(TEXT("Attributes"));

	// Replication
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CombatComponent->WeaponEquippedDelegate.AddUniqueDynamic(this, &ABlasterCharacter::OnWeaponEquipped);
	CombatComponent->WeaponUnequippedDelegate.AddUniqueDynamic(this, &ABlasterCharacter::OnWeaponUnequipped);

	// Bind attributes' value change delegates
	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &ABlasterCharacter::OnMaxMovementSpeedChanged);
	}
}

/** Do any object-specific cleanup required immediately after loading an object. */
void ABlasterCharacter::PostLoad()
{
	Super::PostLoad();
	
	// Set default character data: startup abilities and effects, animation data...
	if (IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}

/** Called when the game starts or when spawned */
void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeCharacter();
}

/** Called every frame */
void ABlasterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaSeconds);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaSeconds; 
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAimOffsetPitch();
	}
	
	HandleCharacterCloseToCamera();
}

/** Called when this Pawn is possessed (only called on the server) */
void ABlasterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveAbilities();
	ApplyStartupEffects();
}

/** PlayerState Replication Notification (only called on the client) */
void ABlasterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
}

/** Tell client that the Pawn is begin restarted */
void ABlasterCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Add default mapping context
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

/** Called upon landing when falling, to perform actions based on the Hit result */
void ABlasterCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
}

/** Make the character jump on the next update */
void ABlasterCharacter::Jump()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}

	Super::Jump();
}

/** Called on client when updated bReplicateMovement value is received for this actor */
void ABlasterCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	
	SimulatedProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

#pragma endregion OVERRIDES

#pragma region INPUT

/** Called to bind functionality to input */
void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		if (InputAction_Move)
		{
			EnhancedInputComponent->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &ABlasterCharacter::InputAction_Move_Triggered);
		}

		// Looking
		if (InputAction_Look)
		{
			EnhancedInputComponent->BindAction(InputAction_Look, ETriggerEvent::Triggered, this, &ABlasterCharacter::InputAction_Look_Triggered);
		}
		
		// Jumping
		if (InputAction_Jump)
		{
			EnhancedInputComponent->BindAction(InputAction_Jump, ETriggerEvent::Started, this, &ABlasterCharacter::InputAction_Jump_Started);
			EnhancedInputComponent->BindAction(InputAction_Jump, ETriggerEvent::Completed, this, &ABlasterCharacter::InputAction_Jump_Completed);
		}

		// Equipping weapon
		if (InputAction_Equip)
		{
			EnhancedInputComponent->BindAction(InputAction_Equip, ETriggerEvent::Started, this, &ABlasterCharacter::InputAction_Equip_Started);
		}

		// Jumping
		if (InputAction_Crouch)
		{
			EnhancedInputComponent->BindAction(InputAction_Crouch, ETriggerEvent::Started, this, &ABlasterCharacter::InputAction_Crouch_Started);
			EnhancedInputComponent->BindAction(InputAction_Crouch, ETriggerEvent::Completed, this, &ABlasterCharacter::InputAction_Crouch_Completed);
		}

		// Aiming
		if (InputAction_Aim)
		{
			EnhancedInputComponent->BindAction(InputAction_Aim, ETriggerEvent::Triggered, this, &ABlasterCharacter::InputAction_Aim_Triggered);
			EnhancedInputComponent->BindAction(InputAction_Aim, ETriggerEvent::Completed, this, &ABlasterCharacter::InputAction_Aim_Completed);
		}

		// Firing
		if (InputAction_Fire)
		{
			EnhancedInputComponent->BindAction(InputAction_Fire, ETriggerEvent::Triggered, this, &ABlasterCharacter::InputAction_Fire_Triggered);
			EnhancedInputComponent->BindAction(InputAction_Fire, ETriggerEvent::Completed, this, &ABlasterCharacter::InputAction_Fire_Completed);
		}
	}
}

/** Called when movement input is triggered  */
void ABlasterCharacter::InputAction_Move_Triggered(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}

	// Get Vector2D value from input action
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// Find out which direction is forward
	const FRotator YawRotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);

	// Add movement in both forward and right directions
	const FVector ForwardDirection = FVector(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
	const FVector RightDirection = FVector(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
	AddMovementInput(ForwardDirection, MovementValue.Y);
	AddMovementInput(RightDirection, MovementValue.X);
}

/** Called when looking input is triggered */
void ABlasterCharacter::InputAction_Look_Triggered(const FInputActionValue& Value)
{
	if (!Controller)
	{
		return;
	}

	// Get Vector2D value from input action
	const FVector2D LookValue = Value.Get<FVector2D>();

	// Add yaw and pitch input to controller
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

/** Called when jump input is started */
void ABlasterCharacter::InputAction_Jump_Started(const FInputActionValue& Value)
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(JumpTags);
}

/** Called when jump input is completed */
void ABlasterCharacter::InputAction_Jump_Completed(const FInputActionValue& Value)
{
	
}

/** Called when equip input is started */
void ABlasterCharacter::InputAction_Equip_Started(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
	else
	{
		ServerInputAction_Equip_Started();
	}
}

/** RPC sent when equip input is started (client sends petition to server for equipping weapon) */
void ABlasterCharacter::ServerInputAction_Equip_Started_Implementation()
{
	CombatComponent->EquipWeapon(OverlappingWeapon);
}

/** Called when crouch input is started */
void ABlasterCharacter::InputAction_Crouch_Started(const FInputActionValue& Value)
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(CrouchTags);
}

/** Called when crouch input is completed */
void ABlasterCharacter::InputAction_Crouch_Completed(const FInputActionValue& Value)
{
	AbilitySystemComponent->CancelAbilities(&CrouchTags);
}

/** Called when aiming input is triggered */
void ABlasterCharacter::InputAction_Aim_Triggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(AimTags);
}

/** Called when aiming input is completed */
void ABlasterCharacter::InputAction_Aim_Completed(const FInputActionValue& Value)
{
	AbilitySystemComponent->CancelAbilities(&AimTags);
}

/** Called when firing input is triggered */
void ABlasterCharacter::InputAction_Fire_Triggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(FireTags);
}

/** Called when firing input is completed */
void ABlasterCharacter::InputAction_Fire_Completed(const FInputActionValue& Value)
{
	AbilitySystemComponent->CancelAbilities(&FireTags);
}

#pragma endregion INPUT

#pragma region CORE

/** Setter of CharacterData */
void ABlasterCharacter::SetCharacterData(const FCharacterData& InCharacterData)
{
	CharacterData = InCharacterData;
}

/** Initialize character */
void ABlasterCharacter::InitializeCharacter()
{
	OverheadWidgetRef = CastChecked<UOverheadWidget>(OverheadWidget->GetUserWidgetObject());
}

/** Handle character's visibility when too close to camera */
void ABlasterCharacter::HandleCharacterCloseToCamera() const
{
	if (!IsLocallyControlled())
	{
		return;
	}

	// Hide character if its closer than set camera's threshold
	const bool bShouldHideCharacter = (FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold;
	GetMesh()->SetVisibility(!bShouldHideCharacter);
	if (const ABaseWeapon* EquippedWeapon = GetEquippedWeapon())
	{
		EquippedWeapon->GetWeaponSkeletalMesh()->SetOwnerNoSee(bShouldHideCharacter);
	}
}

#pragma endregion CORE

#pragma region WEAPON

/** Setter of OverlappingWeapon */
void ABlasterCharacter::SetOverlappingWeapon(ABaseWeapon* InOverlappingWeapon)
{
	// Hide last overlapping weapon
	if (OverlappingWeapon)
	{
		OverlappingWeapon->TogglePickupWidget(false);
	}

	// Show new overlapping weapon
	OverlappingWeapon = InOverlappingWeapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->TogglePickupWidget(true);
		}
	}
}

/** RepNotify for OverlappingWeapon */
void ABlasterCharacter::OnRep_OverlappingWeapon(ABaseWeapon* OldOverlappingWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->TogglePickupWidget(true);
	}

	if (OldOverlappingWeapon)
	{
		OldOverlappingWeapon->TogglePickupWidget(false);
	}
}

/** Functionality performed when a weapon is equipped */
void ABlasterCharacter::OnWeaponEquipped(bool bShouldAffectMovement)
{
	if (bShouldAffectMovement)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
	}

	if (const ABaseWeapon* Weapon = GetEquippedWeapon())
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Weapon->GetWeaponAbility(), 1.f, INDEX_NONE, nullptr));
	}
}

/** Functionality performed when a weapon is unequipped */
void ABlasterCharacter::OnWeaponUnequipped(bool bShouldAffectMovement)
{
	
}

/** Returns whether character has a weapon equipped */
bool ABlasterCharacter::IsWeaponEquipped() const
{
	return CombatComponent->IsWeaponEquipped();
}

/** Returns character's currently equipped weapon */
ABaseWeapon* ABlasterCharacter::GetEquippedWeapon() const
{
	return CombatComponent->GetEquippedWeapon();
}

/** Get hit target */
FVector ABlasterCharacter::GetHitTarget() const
{
	return CombatComponent->HitTarget;
}

/** Returns whether character is aiming */
bool ABlasterCharacter::IsAiming() const
{
	return CombatComponent->IsAiming();
}

/** Getter of AimOffsetYaw */
float ABlasterCharacter::GetAimOffsetYaw() const
{
	return AimOffsetYaw;
}

/** Getter of AimOffsetPitch */
float ABlasterCharacter::GetAimOffsetPitch() const
{
	return AimOffsetPitch;
}

/** Getter of bRotateRootBone */
bool ABlasterCharacter::ShouldRotateRootBone() const
{
	return bShouldRotateRootBone;
}

/** Calculate AimOffset's Yaw and Pitch */
void ABlasterCharacter::AimOffset(float DeltaSeconds)
{
	if (!IsWeaponEquipped())
	{
		return;
	}
	
	const float Speed = GetVelocity().Size2D();
	const bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed > 0.f || bIsInAir)
	{
		// Character's moving -> reset aim offset
		bShouldRotateRootBone = false;
		InitialAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AimOffsetYaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::NotTurning;
	}
	else
	{
		// Character's standing still -> calculate delta aim rotation yaw
		bShouldRotateRootBone = true;
		const FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		const FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, InitialAimRotation);
		AimOffsetYaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::NotTurning)
		{
			InterpAimOffsetYaw = AimOffsetYaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaSeconds);
	}

	CalculateAimOffsetPitch();
}

/** Turn for Simulated Proxies */
void ABlasterCharacter::SimulatedProxiesTurn()
{
	if (!IsWeaponEquipped())
	{
		return;
	}
	
	bShouldRotateRootBone = false;
	const float Speed = GetVelocity().Size2D();
	if (Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::NotTurning;
		return;
	}

	LastProxyRotation = CurrentProxyRotation;
	CurrentProxyRotation = GetActorRotation();
	CurrentProxyRotationYaw = UKismetMathLibrary::NormalizedDeltaRotator(CurrentProxyRotation, LastProxyRotation).Yaw;
	UE_LOG(LogTemp, Warning, TEXT("ABlasterCharacter::SimulatedProxiesTurn - CurrentProxyRotationYaw = %f"), CurrentProxyRotationYaw);

	// Set turn in place's state
	if (FMath::Abs(CurrentProxyRotationYaw) > TurnThreshold)
	{
		if (CurrentProxyRotationYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::Right;
		}
		else if (CurrentProxyRotationYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::NotTurning;
		}

		return;
	}

	TurningInPlace = ETurningInPlace::NotTurning;
}

/** Calculate Aim offset's pitch */
void ABlasterCharacter::CalculateAimOffsetPitch()
{
	AimOffsetPitch = GetBaseAimRotation().Pitch;

	// Map pitch from [270, 360) to [-90, 0)
	if (AimOffsetPitch > 90.f && !IsLocallyControlled())
	{
		const FVector2D InRange(270.f, 360.f);
		const FVector2D OutRange(-90.f, 0.f);
		AimOffsetPitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AimOffsetPitch);
	}
}

/** Turn in place */
void ABlasterCharacter::TurnInPlace(float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning, TEXT("ABlasterCharacter::TurnInPlace - AimOffsetYaw: %f"), AimOffsetYaw);

	if (AimOffsetYaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::Right;
	}
	else if (AimOffsetYaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::Left;
	}

	if (TurningInPlace != ETurningInPlace::NotTurning)
	{
		InterpAimOffsetYaw = FMath::FInterpTo(InterpAimOffsetYaw, 0.f, DeltaSeconds, TurnInPlaceSpeed);
		AimOffsetYaw = InterpAimOffsetYaw;
		if (FMath::Abs(AimOffsetYaw) < ResetTurningAngleThreshold)
		{
			TurningInPlace = ETurningInPlace::NotTurning;
			InitialAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

#pragma endregion WEAPON

#pragma region DAMAGE

/** Functionality performed when a shot is received */
void ABlasterCharacter::OnShotReceived()
{
	MultiCastOnShotReceived();
}

/** Multicast RPC called when a shot is received */
void ABlasterCharacter::MultiCastOnShotReceived_Implementation()
{
	if (!IsWeaponEquipped())
	{
		return;
	}
	
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

#pragma endregion DAMAGE

#pragma region GAS

/** Getter of AbilitySystemComponent */
UAbilitySystemComponent* ABlasterCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/** Give abilities */
void ABlasterCharacter::GiveAbilities() const
{
	AbilitySystemComponent->GiveAbilities(CharacterData.Abilities);
}

/** Apply gameplay effects at startup */
void ABlasterCharacter::ApplyStartupEffects() const
{
	AbilitySystemComponent->ApplyEffects(CharacterData.Effects);
}

/** Function bound to the delegate that is called whenever the MaxMovementSpeed attribute is changed */
void ABlasterCharacter::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

#pragma endregion GAS