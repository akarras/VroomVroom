// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFRaceGameState.h"

AAKZFRaceGameState::AAKZFRaceGameState()
{
	NumberOfLaps = 3;
}

void AAKZFRaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAKZFRaceGameState, NumberOfLaps);
}