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
	bUseSeamlessTravel = true;
}

void AAKZFVroomVroomGameMode::PostLogin(APlayerController* controller)
{
	Super::PostLogin(controller);
	AAKZFRacePlayerController* raceController = Cast<AAKZFRacePlayerController>(controller);
	if (raceController)
	{
		raceController->ConfigureUI();
		ConnectedControllers.Add(raceController);
	}
}

void AAKZFVroomVroomGameMode::SwapPlayerControllers(APlayerController * OldPC, APlayerController * NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	AAKZFRacePlayerController* RaceController = Cast<AAKZFRacePlayerController>(NewPC);
	if (RaceController)
	{
		RaceController->ConfigureUI();
		ConnectedControllers.Add(RaceController);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString("Failed to cast RaceController"));
	}
}

void AAKZFVroomVroomGameMode::Logout(AController* controller)
{
	AAKZFRacePlayerController* raceController = Cast<AAKZFRacePlayerController>(controller);
	if (raceController)
	{
		ConnectedControllers.Remove(raceController);
	}
}

APawn* AAKZFVroomVroomGameMode::SpawnDefaultPawnAtTransform_Implementation(AController * NewPlayer, const FTransform & SpawnTransform)
{
	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	parameters.Instigator = Instigator;
	parameters.ObjectFlags |= RF_Transient;
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	APawn* Actor = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, parameters);
	return Actor;
}

void AAKZFVroomVroomGameMode::SpawnSpectatorForController(AAKZFRacePlayerController* controller)
{
	
}