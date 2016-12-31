// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/PlayerState.h"
#include "Data/MapData.h"
#include "AKZFRacePlayerState.generated.h"

USTRUCT()
struct FCheckpointData
{
	GENERATED_BODY()
	/* Comparison operator for sorting */
	FORCEINLINE bool operator()(const FCheckpointData& A, const FCheckpointData& B) const
	{
		return A.ReachedTime < B.ReachedTime;
	}

	FCheckpointData() {}

	FCheckpointData(int checkpoint, int lap, float reachedTime)
	{
		Checkpoint = checkpoint;
		Lap = lap;
		ReachedTime = reachedTime;
	}

	FCheckpointData(int checkpoint, int lap)
	{
		Checkpoint = checkpoint;
		Lap = lap;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float ReachedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lap")
	int Checkpoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lap")
	int Lap;
};

UCLASS()
class AKZFVROOMVROOM_API AAKZFRacePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AAKZFRacePlayerState();
	/* Indicates the players vote for the next map. Gets counted by the GameState */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "MapVoting")
	int MapVote;
	/* Indicates the next checkpoint for the player to go to */
	UPROPERTY(Replicated, BlueprintReadWrite, Category= "Checkpoint")
	int NextCheckpoint;
	/* Indiciates the checkpoint that the player was last at */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Checkpoint")
	int LastCheckpoint;
	/* Indicates how many laps the player has completed */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Score")
	int LapsComplete;
	/* Indicates if the player has finished or not. */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Score")
	bool HasFinished;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Checkpoints")
	TArray<FCheckpointData> Checkpoints;
};