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
	UPROPERTY(BlueprintReadWrite, Category = "Score")
	int lapsComplete;
	
	
};
