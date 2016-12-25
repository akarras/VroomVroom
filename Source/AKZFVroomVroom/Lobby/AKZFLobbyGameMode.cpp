// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "Data/MapData.h"
#include "AKZFLobbyController.h"
#include "AKZFLobbyPawn.h"
#include "AKZFLobbyGameMode.h"


AAKZFLobbyGameMode::AAKZFLobbyGameMode()
{
	DefaultPawnClass = AAKZFLobbyPawn::StaticClass();
	PlayerControllerClass = AAKZFLobbyController::StaticClass();
	bUseSeamlessTravel = true; // Enable seamless travel because we will be switching to other maps
}

void AAKZFLobbyGameMode::StartGame()
{
	// Start the game by initiating a server travel to the map url
	GetWorld()->ServerTravel(SelectedMap.Url);
}

TArray<FMapInformation> AAKZFLobbyGameMode::LoadMaps()
{
	FStringAssetReference MapReference(FString("MapData'/Game/VroomVroom/Data/Maps.Maps'"));
	UMapData* asset = Cast<UMapData>(MapReference.TryLoad());
	return asset->Maps;
}

void AAKZFLobbyGameMode::PostLogin(APlayerController* controller)
{
	ConnectedPlayers.Add(controller);
}

void AAKZFLobbyGameMode::Logout(AController* controller)
{
	APlayerController* playerController = Cast<APlayerController>(controller);
	if (playerController)
	{
		ConnectedPlayers.Remove(playerController);
	}
}