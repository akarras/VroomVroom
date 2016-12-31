// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFVroomVroomGameMode.h"
#include "AKZFVroomVroomPawn.h"
#include "AKZFRacePlayerController.h"
#include "AKZFRacePlayerState.h"
#include "AKZFGameInstance.h"
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
		UpdateRoundTime(30);
		break;
	case ERaceState::R_TimeToFinish:
		RaceState = ERaceState::R_Finished;
		RaceOver();
		UpdateRoundTime(30);
		break;
	case ERaceState::R_Finished:
		// Might add server travel logic here.
		// Or add a quick voting widget and not a full lobby.
		TravelNextMap();
		break;
	}
}

void AAKZFRaceGameState::StartRace()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Resetting warmup"));
	// Gets the gamemode
	AAKZFVroomVroomGameMode* RaceMode = Cast<AAKZFVroomVroomGameMode>(AuthorityGameMode);
	if (RaceMode)
	{
		// Loops through all connected controller
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
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Race over, swapping UI"));
	// Gets the gamemode
	AAKZFVroomVroomGameMode* RaceMode = Cast<AAKZFVroomVroomGameMode>(AuthorityGameMode);
	if (RaceMode)
	{
		// Loops through all connected controllers
		for (AAKZFRacePlayerController* controller : RaceMode->ConnectedControllers)
		{
			controller->ShowEndGameMenu();
		}
	}
}

void AAKZFRaceGameState::TravelNextMap()
{
	// Go to that awesome map! WOOT
	// Clean up the UI before travel
	AAKZFVroomVroomGameMode* gameMode = Cast<AAKZFVroomVroomGameMode>(AuthorityGameMode);
	if (gameMode)
	{
		for (AAKZFRacePlayerController* controller : gameMode->ConnectedControllers)
		{
			controller->RemoveWidget();
		}
	}

	FMapInformation winningMap = Maps[GetHighestVote()];
	GetWorld()->ServerTravel(winningMap.Url);
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

TArray<int> AAKZFRaceGameState::GetTalliedMapVotes()
{
	TArray<int> MapCounts;
	if (Maps.Num() == 0) // If it's zero it's not loaded yet!
	{
		UAKZFGameInstance* instance = Cast<UAKZFGameInstance>(GetGameInstance());
		if (instance)
		{
			Maps = instance->LoadMaps();
			check(Maps.Num() != 0); // Check it's still not zero... We might have an issue.
		}
	}
	MapCounts.SetNum(Maps.Num()); // Our map voting counts should probably be as big as the number of maps we have! Neat!
	// Iterate through all player states and count em up
	for (APlayerState* state : PlayerArray)
	{
		// Cast to our local player
		AAKZFRacePlayerState* player = Cast<AAKZFRacePlayerState>(state);
		// Increment the vote count for the given number
		if (player->MapVote > 0)
		{
			// Only count the vote if it's in our array
			int MapCount = MapCounts[player->MapVote];
			MapCount = MapCount + 1;
			MapCounts[player->MapVote] = MapCount;
		}
	}
	// Return the voter tally!
	return MapCounts;
}

int AAKZFRaceGameState::GetHighestVote()
{
	// Simple select
	int winningMap = 0; // Default to 0 winning our pick in case we don't win!
	int winningCount = 0;
	TArray<int> MapVotes = GetTalliedMapVotes();
	for (int count = 0; count < MapVotes.Num(); count++)
	{
		int mapCount = MapVotes[count];
		if (mapCount > winningCount)
		{
			// If the winning count is better, this is our winner. Congratulations new map!
			winningMap = count;
			winningCount = mapCount;
		}
	}
	return winningMap;
}