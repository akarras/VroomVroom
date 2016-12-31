// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "UObject/NoExportTypes.h"
#include "FriendBlueprintWrapper.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AKZFVROOMVROOM_API UFriendBlueprintWrapper : public UObject
{
	GENERATED_BODY()
	
public:
	UFriendBlueprintWrapper();

	TSharedPtr<FOnlineFriend> Friend;
	UFUNCTION(BlueprintCallable, Category = "Friend")
	FString GetUsername();
	/* Returns Online, Offline, In Game, Joinable, ect. */
	UFUNCTION(BlueprintCallable, Category = "Friend")
	FString GetOnlineStatus();
	
};
