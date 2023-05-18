// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "General/Structs/Data/CharacterData.h"

#include "DataAsset_CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class BLASTERSHOOTER_API UDataAsset_CharacterData : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Character's data */
	UPROPERTY(EditDefaultsOnly)
	FCharacterData CharacterData;
	
};
