// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "UnrealNetwork.h"
#include "AKZFLobbyGameState.h"


void AAKZFLobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME(AAKZFLobbyGameState, CurrentMap);
}

