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
}

void AAKZFVroomVroomGameMode::PostLogin(APlayerController* controller)
{
	Super::PostLogin(controller);
	AAKZFRacePlayerController* raceController = Cast<AAKZFRacePlayerController>(controller);
	//raceController->ConfigureUI();
}