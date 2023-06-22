// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Casings/BaseCasing.h"

// Unreal Engine
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// BlasterShooter
#include "General/DataAssets/DataAsset_CasingData.h"

#pragma region INITIALIZATION

/** Sets default values for this actor's properties */
ABaseCasing::ABaseCasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	RootComponent = CasingMesh;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES

/** Allow actors to initialize themselves on the C++ side after all of their components have been initialized, only called during gameplay */
void ABaseCasing::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CasingMesh->OnComponentHit.AddUniqueDynamic(this, &ABaseCasing::OnHit);
}

/** Called when the game starts or when spawned */
void ABaseCasing::BeginPlay()
{
	Super::BeginPlay();
	LaunchCasing(CasingData.EjectionImpulse);
}

#pragma endregion OVERRIDES

#pragma region CASING

/** Initialize casing's values */
void ABaseCasing::InitializeCasing(const UDataAsset_CasingData* CasingDataAsset)
{
	if (!CasingDataAsset)
	{
		return;
	}
	
	CasingData = CasingDataAsset->CasingData;

	if (CasingData.StaticMesh)
	{
		CasingMesh->SetStaticMesh(CasingData.StaticMesh);
	}
}

/** Launch casing when spawned */
void ABaseCasing::LaunchCasing(float Impulse)
{
	CasingMesh->AddImpulse(GetActorForwardVector() * Impulse);
}

/** Handle casing's hit */
void ABaseCasing::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	CasingMesh->SetSimulatePhysics(false);
	CasingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (CasingData.HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CasingData.HitSound, GetActorLocation());
	}
	
	SetLifeSpan(CasingData.Lifespan);
}

#pragma endregion CASING

