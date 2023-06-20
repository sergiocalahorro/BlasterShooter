// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ProjectileWeapon.h"

// Unreal Engine
#include "Engine/SkeletalMeshSocket.h"

// BlasterShooter
#include "Weapon/Projectiles/BaseProjectile.h"

#pragma region WEAPON

/** Fire weapon */
void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	if (!HasAuthority())
	{
		return;
	}
	
	APawn* InstigatorPawn = CastChecked<APawn>(GetOwner());
	if (const USkeletalMeshComponent* WeaponSkeletalMesh = GetWeaponSkeletalMesh())
	{
		if (const USkeletalMeshSocket* MuzzleFlashSocket = WeaponSkeletalMesh->GetSocketByName(WeaponDataAsset->WeaponData.MuzzleFlashSocketName))
		{
			if (WeaponDataAsset->WeaponData.ProjectileClass)
			{
				const FVector ProjectileLocation = MuzzleFlashSocket->GetSocketLocation(WeaponSkeletalMesh);
				const FRotator ProjectileRotation = (HitTarget - ProjectileLocation).GetSafeNormal().Rotation();
				const FTransform SpawnTransform = FTransform(ProjectileRotation, ProjectileLocation, FVector::OneVector);
				if (ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(WeaponDataAsset->WeaponData.ProjectileClass, SpawnTransform, GetOwner(), InstigatorPawn))
				{
					Projectile->InitializeProjectile(ProjectileDataAsset);
					Projectile->FinishSpawning(SpawnTransform);
				}
			}
		}
	}
}

#pragma endregion WEAPON