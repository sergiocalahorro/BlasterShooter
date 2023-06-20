// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Projectiles/BaseProjectile.h"

// Unreal Engine
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// BlasterShooter
#include "General/DataAssets/DataAsset_ProjectileData.h"
#include "General/Structs/Data/ProjectileData.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Setup hierarchy
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RootComponent = BoxCollision;

	// Setup components
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Called when the game starts or when spawned */
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

/** Called every frame */
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma endregion OVERRIDES

#pragma region PROJECTILE

/** Initialize projectile's values */
void ABaseProjectile::InitializeProjectile(const UDataAsset_ProjectileData* ProjectileDataAsset)
{
	if (!ProjectileDataAsset)
	{
		return;
	}
	
	const FProjectileData ProjectileData = ProjectileDataAsset->ProjectileData;

	if (ProjectileData.Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			ProjectileData.Tracer,
			BoxCollision,
			NAME_None,
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}

	// Projectile movement
	ProjectileMovementComponent->InitialSpeed = ProjectileData.InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileData.MaxSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = ProjectileData.GravityScale;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = ProjectileData.bInitialVelocityInLocalSpace;
	ProjectileMovementComponent->bRotationFollowsVelocity = ProjectileData.bRotationFollowsVelocity;
	ProjectileMovementComponent->bShouldBounce = ProjectileData.bShouldBounce;
	ProjectileMovementComponent->Bounciness = ProjectileData.Bounciness;
}

#pragma endregion PROJECTILE
