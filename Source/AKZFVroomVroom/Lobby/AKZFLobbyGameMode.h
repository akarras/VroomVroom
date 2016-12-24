// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/GameModeBase.h"
#include "AKZFLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API AAKZFLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AAKZFLobbyGameMode();
	
	void StartGame();
	
};
