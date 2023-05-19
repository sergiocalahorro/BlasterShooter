// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/HUD/OverheadWidget.h"

// Unreal Engine
#include "Components/TextBlock.h"

#pragma region OVERRIDES

/** Destruct widget */
void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

#pragma endregion OVERRIDES

#pragma region OVERHEAD

/** Set text for DisplayText */
void UOverheadWidget::SetDisplayText(const FString& TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

/** Show incoming player's net role */
void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	if (!InPawn)
	{
		return;
	}
	
	const ENetRole LocalRole = InPawn->GetLocalRole();
	
	FString Role;
	switch (LocalRole)
	{
	case ROLE_Authority:
		Role = FString("Authority");
		break;
		
	case ROLE_SimulatedProxy: 
		Role = FString("SimulatedProxy");
		break;
		
	case ROLE_AutonomousProxy:
		Role = FString("AutonomousProxy");
		break;
	
	case ROLE_None:
		Role = FString("None");
		break;
	}
	
	const FString LocalRoleString = FString::Printf(TEXT("Local Role: %s"), *Role);
	SetDisplayText(LocalRoleString);
}

#pragma endregion OVERHEAD