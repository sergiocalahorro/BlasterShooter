// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HUD/BlasterHUD.h"

// Unreal Engine
#include "GameFramework/Character.h"

// BlasterShooter
#include "General/Components/CombatComponent.h"

#pragma region OVERRIDES

/** Called when the game starts or when spawned */
void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (const APlayerController* Controller = GetOwningPlayerController())
	{
		if (const ACharacter* Character = Controller->GetCharacter())
		{
			CombatComponent = Cast<UCombatComponent>(Character->FindComponentByClass(UCombatComponent::StaticClass()));
			if (CombatComponent)
			{
				CombatComponent->CrosshairsUpdateDelegate.AddUniqueDynamic(this, &ABlasterHUD::SetHUDPackage);
				CombatComponent->CrosshairsTraceHitDelegate.AddUniqueDynamic(this, &ABlasterHUD::CrosshairsTraceHit);
				CombatComponent->WeaponStartedFireDelegate.AddUniqueDynamic(this, &ABlasterHUD::WeaponStartedFire);
				CombatComponent->WeaponStoppedFireDelegate.AddUniqueDynamic(this, &ABlasterHUD::WeaponStoppedFire);
			}
		}
	}
}

/** The Main Draw loop for the hud. Gets called before any messaging */
void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		const float SpreadScaled = CrosshairMaxSpread * HUDPackage.CrosshairSpread;
		DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, FVector2D::ZeroVector, HUDPackage.CrosshairColor);
		DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, FVector2D(-SpreadScaled, 0.f), HUDPackage.CrosshairColor);
		DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, FVector2D(SpreadScaled, 0.f), HUDPackage.CrosshairColor);
		DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, FVector2D(0.f, -SpreadScaled), HUDPackage.CrosshairColor);
		DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, FVector2D(0.f, SpreadScaled), HUDPackage.CrosshairColor);
	}
}

#pragma endregion OVERRIDES

#pragma region HUD

/** Setter of HUDPackage */
void ABlasterHUD::SetHUDPackage(const FHUDPackage& InHUDPackage)
{
	HUDPackage = InHUDPackage;
}

/** Crosshairs' trace hit event */
void ABlasterHUD::CrosshairsTraceHit(bool bReactToCrosshair)
{
	HUDPackage.CrosshairColor = bReactToCrosshair ? CrosshairHitColor : CrosshairDefaultColor;
	if (bIsFiring)
	{
		HUDPackage.CrosshairColor = CrosshairFiringColor;
	}
}

/** Setter of HUDPackage */
void ABlasterHUD::WeaponStartedFire()
{
	bIsFiring = true;
	HUDPackage.CrosshairColor = CrosshairFiringColor;
}

/** Weapon stopped fire's event */
void ABlasterHUD::WeaponStoppedFire()
{
	bIsFiring = false;
	HUDPackage.CrosshairColor = CrosshairDefaultColor;
}

/** Draw crosshair */
void ABlasterHUD::DrawCrosshair(UTexture2D* Crosshair, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	if (!Crosshair)
	{
		return;
	}

	const float CrosshairWidth = Crosshair->GetSizeX();
	const float CrosshairHeight = Crosshair->GetSizeY();
	const FVector2D CrosshairDrawPoint(
		ViewportCenter.X - (CrosshairWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (CrosshairHeight / 2.f) + Spread.Y
	);
	
	DrawTexture(Crosshair, CrosshairDrawPoint.X, CrosshairDrawPoint.Y, CrosshairWidth, CrosshairHeight, 0.f, 0.f, 1.f, 1.f, CrosshairColor);
}

#pragma endregion HUD

