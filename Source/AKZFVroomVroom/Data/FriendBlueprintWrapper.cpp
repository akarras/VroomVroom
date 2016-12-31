// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "FriendBlueprintWrapper.h"

UFriendBlueprintWrapper::UFriendBlueprintWrapper()
{
	// Empty constructor. Lel.
}

FString UFriendBlueprintWrapper::GetUsername()
{
	return Friend->GetRealName();
}

FString UFriendBlueprintWrapper::GetOnlineStatus()
{
	// Return a string status that makes us the most happy!
	FString status; 
	if (Friend->GetPresence().bIsJoinable)
	{
		status = FString("Joinable");
	}
	else if (Friend->GetPresence().bIsPlayingThisGame)
	{
		status = FString("Also vrooming");
	}
	else if (Friend->GetPresence().bIsOnline)
	{
		status = FString("Online");
	}
	else
	{
		status = FString("Offline");
	}
	return status;
}