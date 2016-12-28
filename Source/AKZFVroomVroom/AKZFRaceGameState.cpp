// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFVroomVroomGameMode.h"
#include "AKZFRacePlayerController.h"
#include "AKZFRacePlayerState.h"
#include "AKZFRaceGameState.h"

AAKZFRaceGameState::AAKZFRaceGameState()
{
	NumberOfLaps = 3;
	PrimaryActorTick.bCanEverTick = true;
}

void AAKZFRaceGameState::BeginPlay()
{
	Super::BeginPlay();
	if (Role >= ROLE_Authority)
	{
		RaceState = ERaceState::R_Warmup;
		UpdateRoundTime(15.0f);
	}
}

void AAKZFRaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAKZFRaceGameState, NumberOfLaps);
	DOREPLIFETIME(AAKZFRaceGameState, RaceState);
	DOREPLIFETIME(AAKZFRaceGameState, StateRoundLimit);
}

void AAKZFRaceGameState::AdvanceState()
{
	switch (RaceState)
	{
	case ERaceState::R_Warmup:
		RaceState = ERaceState::R_Started;
		// Restart the game for the game start
		DoReset();
		UpdateRoundTime(720.0f);
		break;
	case ERaceState::R_Started:
		RaceState = ERaceState::R_TimeToFinish;
		UpdateRoundTime(60);
		break;
	case ERaceState::R_TimeToFinish:
		RaceState = ERaceState::R_Finished;
		UpdateRoundTime(30);
		break;
	case ERaceState::R_Finished:
		// Might add server travel logic here.
		// Or add a quick voting widget and not a full lobby.
		break;
	}
}

void AAKZFRaceGameState::DoReset()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Resetting warmup"));

	AAKZFVroomVroomGameMode* RaceMode = Cast<AAKZFVroomVroomGameMode>(AuthorityGameMode);
	if (RaceMode)
	{
		RaceMode->ResetLevel();
		for (AAKZFRacePlayerController* controller : RaceMode->ConnectedControllers)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Resetting a player?"));
			RaceMode->RestartPlayer(controller);
		}
		for (APlayerState* State : PlayerArray)
		{
			AAKZFRacePlayerState* playerState = Cast<AAKZFRacePlayerState>(State);
			playerState->Reset();
		}
	}
}

void AAKZFRaceGameState::UpdateRoundTime(float time)
{
	StateRoundLimit = GetServerWorldTimeSeconds() + time;
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &AAKZFRaceGameState::RoundTimerUp, time);
}

void AAKZFRaceGameState::RoundTimerUp()
{
	AdvanceState(); // It advances the state! Such amazing technology.
}