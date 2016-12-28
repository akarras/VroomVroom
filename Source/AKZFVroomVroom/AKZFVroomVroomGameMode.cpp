// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AKZFVroomVroom.h"
#include "AKZFVroomVroomGameMode.h"
#include "AKZFVroomVroomPawn.h"
#include "AKZFRacePlayerState.h"
#include "AKZFRacePlayerController.h"
#include "AKZFRaceGameState.h"
#include "AKZFVroomVroomHud.h"

AAKZFVroomVroomGameMode::AAKZFVroomVroomGameMode()
{
	DefaultPawnClass = AAKZFVroomVroomPawn::StaticClass();
	HUDClass = AAKZFVroomVroomHud::StaticClass();
	PlayerControllerClass = AAKZFRacePlayerController::StaticClass();
	PlayerStateClass = AAKZFRacePlayerState::StaticClass();
	GameStateClass = AAKZFRaceGameState::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}

void AAKZFVroomVroomGameMode::PostLogin(APlayerController* controller)
{
	Super::PostLogin(controller);
	AAKZFRacePlayerController* raceController = Cast<AAKZFRacePlayerController>(controller);
	raceController->ConfigureUI();
	
}

void AAKZFVroomVroomGameMode::SwapPlayerControllers(APlayerController * OldPC, APlayerController * NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	AAKZFRacePlayerController* RaceController = Cast<AAKZFRacePlayerController>(NewPC);
	if (RaceController)
	{
		RaceController->ConfigureUI();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString("Failed to cast RaceController"));
	}

}

void AAKZFVroomVroomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AAKZFRaceGameState* RaceState = Cast<AAKZFRaceGameState>(GameState);
	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, FString("Gamemode tick"));
	TArray<APlayerState*> players = RaceState->PlayerArray;
	int position = 1;
	for (APlayerState* player : players)
	{
		
	}
}