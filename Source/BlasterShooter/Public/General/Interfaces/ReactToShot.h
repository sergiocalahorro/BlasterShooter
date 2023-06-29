// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ReactToShot.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UReactToShot : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLASTERSHOOTER_API IReactToShot
{
	GENERATED_BODY()

public:

	/** Functionality performed when a shot is received */
	UFUNCTION()
	virtual void OnShotReceived() PURE_VIRTUAL(IReactToShot::OnShotReceived);
};
