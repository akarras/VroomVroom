// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "UObject/NoExportTypes.h"
#include "SessionSearchResultWrapper.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AKZFVROOMVROOM_API USessionSearchResultWrapper : public UObject
{
	GENERATED_BODY()
	
public:
	USessionSearchResultWrapper();
	FOnlineSessionSearchResult SessionResult;
	USessionSearchResultWrapper(FOnlineSessionSearchResult Session);
	UFUNCTION(BlueprintCallable, Category="SessionDetails")
	FString GetOwnerName();
};
