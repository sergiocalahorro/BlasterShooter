// Fill out your copyright notice in the Description page of Project Settings.

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

// BlasterShooter
#include "Components/WidgetComponent.h"
#include "General/DataAssets/DataAsset_CharacterData.h"
#include "GAS/AbilitySystem/BlasterAbilitySystemComponent.h"
#include "GAS/Attributes/BlasterAttributeSet.h"
#include "General/Components/CombatComponent.h"
#include "UI/HUD/OverheadWidget.h"
#include "Weapon/WeaponActor.h"

#pragma region INITIALIZATION

/** Sets default values for this character's properties */
ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup hierarchy
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	CameraBoom->TargetArmLength = 600.f;
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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	
	// GAS setup
	AbilitySystemComponent = CreateDefaultSubobject<UBlasterAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UBlasterAttributeSet>(TEXT("Attributes"));
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CombatComponent->SetBlasterCharacter(this);
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
	AbilitySystemComponent->CancelAbilities(&JumpTags);
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
	// OverheadWidgetRef->ShowPlayerNetRole(this);
}

#pragma endregion CORE

#pragma region WEAPON

/** Setter of OverlappingWeapon */
void ABlasterCharacter::SetOverlappingWeapon(AWeaponActor* InOverlappingWeapon)
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
void ABlasterCharacter::OnRep_OverlappingWeapon(AWeaponActor* OldOverlappingWeapon)
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

/** Returns whether character has a weapon equipped */
bool ABlasterCharacter::IsWeaponEquipped() const
{
	return CombatComponent->IsWeaponEquipped();
}

#pragma endregion WEAPON

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

#pragma endregion GAS