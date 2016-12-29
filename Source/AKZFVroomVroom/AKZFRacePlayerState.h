// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/PlayerState.h"
#include "Data/MapData.h"
#include "AKZFRacePlayerState.generated.h"

USTRUCT()
struct FCheckpointStruct
{
	GENERATED_BODY()
	/* Comparison operator for sorting */
	FORCEINLINE bool operator()(const FCheckpointStruct& A, const FCheckpointStruct& B) const
	{
		return A.ReachedTime < B.ReachedTime;
	}
	float ReachedTime;
	int Checkpoint;
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
	FMapInformation MapVote;
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
};