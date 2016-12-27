// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/GameStateBase.h"
#include "AKZFRaceGameState.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API AAKZFRaceGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AAKZFRaceGameState();

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Laps")
	int NumberOfLaps;
};
