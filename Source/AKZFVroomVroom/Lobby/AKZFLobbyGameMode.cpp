// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "Data/MapData.h"
#include "AKZFLobbyController.h"
#include "AKZFLobbyPawn.h"
#include "AKZFLobbyGameState.h"
#include "AKZFLobbyPlayerState.h"
#include "AKZFLobbyGameMode.h"


AAKZFLobbyGameMode::AAKZFLobbyGameMode()
{
	DefaultPawnClass = AAKZFLobbyPawn::StaticClass();
	PlayerControllerClass = AAKZFLobbyController::StaticClass();
	GameStateClass = AAKZFLobbyGameState::StaticClass();
	PlayerStateClass = AAKZFLobbyPlayerState::StaticClass();
	bUseSeamlessTravel = true; // Enable seamless travel because we will be switching to other maps
}

void AAKZFLobbyGameMode::StartGame()
{
	// Start the game by initiating a server travel to the map url
	AAKZFLobbyGameState* gameState = Cast<AAKZFLobbyGameState>(GameState);
	if (gameState)
	{
		// Instructs all of the lobby controllers to remove their widget.
		for (AAKZFLobbyController* controller : ConnectedPlayers)
		{
			controller->RemoveUserWidget();
		}
		// Travel to the new level
		if (GetWorld()->ServerTravel(gameState->CurrentMap.Url))
		{
			GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Traveling!"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Failed, likely playing in PIE. Cannot do seamless travel from PIE"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Game state error"));
	}
}

TArray<FMapInformation> AAKZFLobbyGameMode::LoadMaps()
{
	FStringAssetReference MapReference(FString("MapData'/Game/VroomVroom/Data/Maps.Maps'"));
	UMapData* asset = Cast<UMapData>(MapReference.TryLoad());
	return asset->Maps;
}

void AAKZFLobbyGameMode::PostLogin(APlayerController* controller)
{
	AAKZFLobbyController* lobbyPC = Cast<AAKZFLobbyController>(controller);
	if (lobbyPC)
	{
		lobbyPC->ConfigureUI();
		ConnectedPlayers.Add(lobbyPC);
	}
}

void AAKZFLobbyGameMode::Logout(AController* controller)
{
	AAKZFLobbyController* playerController = Cast<AAKZFLobbyController>(controller);
	if (playerController)
	{
		ConnectedPlayers.Remove(playerController);
	}
}
