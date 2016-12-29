// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFRacePlayerState.h"
#include "UnrealNetwork.h"

AAKZFRacePlayerState::AAKZFRacePlayerState()
{
	bReplicates = true;
}

void AAKZFRacePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAKZFRacePlayerState, MapVote);
	DOREPLIFETIME(AAKZFRacePlayerState, NextCheckpoint);
	DOREPLIFETIME(AAKZFRacePlayerState, LastCheckpoint);
	DOREPLIFETIME(AAKZFRacePlayerState, LapsComplete);
	DOREPLIFETIME(AAKZFRacePlayerState, HasFinished);
}