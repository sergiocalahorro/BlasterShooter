// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "General/Structs/HUD/HUDPackage.h"

#include "BlasterHUD.generated.h"

class UCombatComponent;

UCLASS()
class BLASTERSHOOTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

#pragma region OVERRIDES
	
public:

	/** The Main Draw loop for the hud. Gets called before any messaging */
	virtual void DrawHUD() override;

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

#pragma endregion OVERRIDES

#pragma region HUD

private:

	/** Setter of HUDPackage */
	UFUNCTION()
	void SetHUDPackage(const FHUDPackage& InHUDPackage);

	/** Draw crosshair */
	void DrawCrosshair(UTexture2D* Crosshair, FVector2D ViewportCenter, FVector2D Spread = FVector2D::ZeroVector);

private:

	/** CombatComponent's reference */
	UPROPERTY()
	TObjectPtr<UCombatComponent> CombatComponent;

	/** HUD Package containing textures for crosshairs */
	FHUDPackage HUDPackage;

	/** Max spread crosshairs can support */
	UPROPERTY(EditDefaultsOnly)
	float CrosshairMaxSpread = 16.f;

#pragma endregion HUD
	
};
