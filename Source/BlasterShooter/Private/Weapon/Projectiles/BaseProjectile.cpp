// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Projectiles/BaseProjectile.h"

// Unreal Engine
#include "Components/BoxComponent.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup hierarchy
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	RootComponent = BoxCollision;
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
