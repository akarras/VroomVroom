// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/GameStateBase.h"
#include "Data/MapData.h"
#include "AKZFLobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API AAKZFLobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Map")
	FMapInformation CurrentMap;
	
	
};
