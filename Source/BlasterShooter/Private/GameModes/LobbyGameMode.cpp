// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/LobbyGameMode.h"

// Unreal Engine
#include "GameFramework/GameStateBase.h"

#pragma region INITIALIZATION

/** Constructor */
ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

#pragma endregion INITIALIZATION

#pragma region OVERRIDES
	
/** Called after a successful login */
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == MaxPlayersToTravelToLobby)
	{
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel(PathToLobby);
		}
	}
}

#pragma endregion OVERRIDES