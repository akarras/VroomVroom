// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFVroomVroomGameMode.h"
#include "AKZFVroomVroomPawn.h"
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
		// Starts the race state timer
		RaceState = ERaceState::R_Warmup; // The game should start in Warmup
		UpdateRoundTime(5.0f); // The warmup should only last five seconds.
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
		StartRace();
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

void AAKZFRaceGameState::StartRace()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Resetting warmup"));

	AAKZFVroomVroomGameMode* RaceMode = Cast<AAKZFVroomVroomGameMode>(AuthorityGameMode);
	if (RaceMode)
	{
		for (AAKZFRacePlayerController* controller : RaceMode->ConnectedControllers)
		{
			AAKZFVroomVroomPawn* racePawn = Cast<AAKZFVroomVroomPawn>(controller->GetPawn());
			if (racePawn)
			{
				racePawn->bCanMove = true; // Tell all of our players that they are now allowed to move.
			}
		}
	}
}

void AAKZFRaceGameState::RaceOver()
{

}

void AAKZFRaceGameState::TravelNextMap()
{

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

TMap<FString, int> AAKZFRaceGameState::GetTalliedMapVotes()
{
	TMap<FString, int> Maps;
	// Iterate through all player states
	for (APlayerState* state : PlayerArray)
	{
		// Cast to our local player
		/*AAKZFRacePlayerState* player = Cast<AAKZFRacePlayerState>(state);
		if (Maps.Contains(player->MapVote))
		{
			int votes = *Maps.Find(player->MapVote); // Get the votes for the given map
			Maps.Emplace(player->MapVote, votes + 1); // Add one to our votes and set it back into our maps.
		}
		else
		{
			Maps.Emplace(player->MapVote, 1); // No one else has voted for this map, so add it to our maps and give it a vote.
		}*/
	}
	// Return the voter tally!
	return Maps;
}