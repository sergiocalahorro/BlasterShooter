// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HUD/BlasterHUD.h"

// Unreal Engine
#include "GameFramework/Character.h"

// BlasterShooter
#include "General/Components/CombatComponent.h"

#pragma region OVERRIDES

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
		DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter);
		DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, FVector2D(-SpreadScaled, 0.f));
		DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, FVector2D(SpreadScaled, 0.f));
		DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, FVector2D(0.f, -SpreadScaled));
		DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, FVector2D(0.f, SpreadScaled));
	}
}

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
			}
		}
	}
}

#pragma endregion OVERRIDES

#pragma region HUD

/** Setter of HUDPackage */
void ABlasterHUD::SetHUDPackage(const FHUDPackage& InHUDPackage)
{
	HUDPackage = InHUDPackage;
}

/** Draw crosshair */
void ABlasterHUD::DrawCrosshair(UTexture2D* Crosshair, FVector2D ViewportCenter, FVector2D Spread)
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
	
	DrawTexture(Crosshair, CrosshairDrawPoint.X, CrosshairDrawPoint.Y, CrosshairWidth, CrosshairHeight, 0.f, 0.f, 1.f, 1.f, FLinearColor::White);
}

#pragma endregion HUD

