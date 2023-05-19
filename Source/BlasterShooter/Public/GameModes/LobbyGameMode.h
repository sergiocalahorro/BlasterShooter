// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Unreal Engine
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "LobbyGameMode.generated.h"

UCLASS()
class BLASTERSHOOTER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

#pragma region INITIALIZATION

public:

	/** Constructor */
	ALobbyGameMode();

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
public:

	/** Called after a successful login */
	virtual void PostLogin(APlayerController* NewPlayer) override;

#pragma endregion OVERRIDES

#pragma region LOBBY

protected:

	/** Maximum number of players in order to travel to lobby level */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Lobby")
	int32 MaxPlayersToTravelToLobby = 2;

	/** Path to lobby level */
	UPROPERTY(EditDefaultsOnly, Category = "AA|Lobby")
	FString PathToLobby = FString("/Game/BlasterShooter/Maps/BlasterMap?listen");

#pragma endregion LOBBY
	
};
