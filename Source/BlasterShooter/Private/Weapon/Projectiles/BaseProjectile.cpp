// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Projectiles/BaseProjectile.h"

// Unreal Engine
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

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

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void ABaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (HasAuthority())
	{
		BoxCollision->OnComponentHit.AddUniqueDynamic(this, &ABaseProjectile::OnHit);
	}
}

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

/** Called when this actor is explicitly being destroyed during gameplay or in the editor, not called during level streaming or gameplay ending */
void ABaseProjectile::Destroyed()
{
	if (ProjectileData.ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileData.ImpactParticles, GetActorTransform());
	}

	if (ProjectileData.ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ProjectileData.ImpactSound, GetActorLocation());
	}

	Super::Destroyed();
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
	
	ProjectileData = ProjectileDataAsset->ProjectileData;

	if (ProjectileData.TracerParticles)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			ProjectileData.TracerParticles,
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

/** Handle projectile's hit */
void ABaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

#pragma endregion PROJECTILE
