// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/BaseWeapon.h"

// Forward declarations - Unreal Engine
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Forward declarations - BlasterShooter
#include "Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "General/DataAssets/DataAsset_CasingData.h"
#include "General/DataAssets/DataAsset_WeaponData.h"
#include "Weapon/Casings/BaseCasing.h"

#pragma region INITIALIZATION

/** Sets default values */
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Setup hierarchy
	PickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger"));
	PickupTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = PickupTrigger;

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(PickupTrigger);
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when an instance of this class is placed (in editor) or spawned */
void ABaseWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Initialize();
}

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

/** Called when the game starts or when spawned */
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PickupTrigger->SetCollisionEnabled((ECollisionEnabled::QueryAndPhysics));
		PickupTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		PickupTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseWeapon::OnPickupTriggerBeginOverlap);
		PickupTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &ABaseWeapon::OnPickupTriggerEndOverlap);
	}

	TogglePickupWidget(false);
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeapon, WeaponState);
}

#pragma endregion OVERRIDES

#pragma region WEAPON

/** Initialize */
void ABaseWeapon::Initialize()
{
	if (!IsValid(WeaponDataAsset))
	{
		return;
	}

	if (WeaponDataAsset->WeaponData.SkeletalMesh)
	{
		if (USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this, USkeletalMeshComponent::StaticClass(), TEXT("WeaponMesh")))
		{
			SkeletalMeshComponent->RegisterComponent();
			SkeletalMeshComponent->SetSkeletalMesh(WeaponDataAsset->WeaponData.SkeletalMesh);
			SkeletalMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			WeaponMesh = SkeletalMeshComponent;
		}
	}
	else if (WeaponDataAsset->WeaponData.StaticMesh)
	{
		if (UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), TEXT("WeaponMesh")))
		{
			StaticMeshComponent->RegisterComponent();
			StaticMeshComponent->SetStaticMesh(WeaponDataAsset->WeaponData.StaticMesh);
			StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			WeaponMesh = StaticMeshComponent;
		}
	}

	// Setup collision
	if (WeaponMesh)
	{
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
		WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

/** Function bound to pickup trigger's begin overlap event */
void ABaseWeapon::OnPickupTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABlasterCharacter* Character = Cast<ABlasterCharacter>(OtherActor))
	{
		Character->SetOverlappingWeapon(this);
	}
}

/** Function bound to pickup trigger's end overlap event */
void ABaseWeapon::OnPickupTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABlasterCharacter* Character = Cast<ABlasterCharacter>(OtherActor))
	{
		Character->SetOverlappingWeapon(nullptr);
	}
}

/** Fire weapon */
void ABaseWeapon::Fire(const FVector& HitTarget)
{
	// Play weapon's mesh animation
	if (WeaponDataAsset->WeaponData.FireAnimation)
	{
		USkeletalMeshComponent* WeaponSkeletalMesh = CastChecked<USkeletalMeshComponent>(WeaponMesh);
		WeaponSkeletalMesh->PlayAnimation(WeaponDataAsset->WeaponData.FireAnimation, false);
	}

	// Spawn casing ejected
	if (const USkeletalMeshComponent* WeaponSkeletalMesh = GetWeaponSkeletalMesh())
	{
		if (const USkeletalMeshSocket* AmmoEjectSocket = WeaponSkeletalMesh->GetSocketByName(WeaponDataAsset->WeaponData.AmmoEjectSocketName))
		{
			if (WeaponDataAsset->WeaponData.CasingClass)
			{
				FTransform CasingSpawnTransform = AmmoEjectSocket->GetSocketTransform(WeaponSkeletalMesh);
				const FRotator RandomSpawnRotation = CasingSpawnTransform.GetRotation().Rotator() +
													 FRotator(FMath::RandRange(CasingDataAsset->CasingData.MinSpawnAngleOffset, CasingDataAsset->CasingData.MaxSpawnAngleOffset),
															  FMath::RandRange(CasingDataAsset->CasingData.MinSpawnAngleOffset, CasingDataAsset->CasingData.MaxSpawnAngleOffset),
															  0.f);
				CasingSpawnTransform.SetRotation(RandomSpawnRotation.Quaternion());
				if (ABaseCasing* Casing = GetWorld()->SpawnActorDeferred<ABaseCasing>(WeaponDataAsset->WeaponData.CasingClass, CasingSpawnTransform, this, nullptr))
				{
					Casing->InitializeCasing(CasingDataAsset);
					Casing->FinishSpawning(CasingSpawnTransform);
				}
			}
		}
	}
}

/** Show/hide pickup widget */
void ABaseWeapon::TogglePickupWidget(bool bShowWidget) const
{
	PickupWidget->SetVisibility(bShowWidget);
}

/** Setter of WeaponState */
void ABaseWeapon::SetWeaponState(EWeaponState InWeaponState)
{
	WeaponState = InWeaponState;
	switch (WeaponState)
	{
	case EWeaponState::None:
		TogglePickupWidget(true);
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EWeaponState::Equipped:
		TogglePickupWidget(false);
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EWeaponState::Dropped:
		TogglePickupWidget(true);
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

/** RepNotify for WeaponState */
void ABaseWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::None:
		TogglePickupWidget(true);
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;

	case EWeaponState::Equipped:
		TogglePickupWidget(false);
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
		
	case EWeaponState::Dropped:
		TogglePickupWidget(true);
		PickupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

/** Get Weapon's ability */
TSubclassOf<UGameplayAbility> ABaseWeapon::GetWeaponAbility() const
{
	if (WeaponDataAsset)
	{
		return WeaponDataAsset->WeaponData.WeaponAbility;
	}

	return nullptr;
}

#pragma endregion WEAPON
