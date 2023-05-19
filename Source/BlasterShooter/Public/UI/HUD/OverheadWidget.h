// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "OverheadWidget.generated.h"

// Forward declarations - Unreal Engine
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region OVERRIDES

protected:

	/** Destruct widget */
	virtual void NativeDestruct() override;

#pragma endregion OVERRIDES

#pragma region OVERHEAD

public:

	/** Set text for DisplayText */
	void SetDisplayText(const FString& TextToDisplay);

	/** Show incoming player's net role */
	void ShowPlayerNetRole(APawn* InPawn);

public:

	/** Text displayed over head */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;

#pragma endregion OVERHEAD
	
};
