// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/PlayerState.h"
#include "AKZFRacePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API AAKZFRacePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AAKZFRacePlayerState();

	UPROPERTY(Replicated, BlueprintReadWrite, Category= "Checkpoint")
	int NextCheckpoint;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Checkpoint")
	int LastCheckpoint;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Score")
	int LapsComplete;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Score")
	bool HasFinished;
};