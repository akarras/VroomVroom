// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/GameStateBase.h"
#include "RaceState.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API ARaceState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category = "Score")
	int Laps;
	
	
};
