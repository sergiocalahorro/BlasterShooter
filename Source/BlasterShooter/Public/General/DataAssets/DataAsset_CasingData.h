// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// BlasterShooter
#include "General/Structs/Data/CasingData.h"

#include "DataAsset_CasingData.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UDataAsset_CasingData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Casing's data */
	UPROPERTY(EditDefaultsOnly)
	FCasingData CasingData;
};
