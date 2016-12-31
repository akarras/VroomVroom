// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "Engine/GameInstance.h"
#include "Data/SessionSearchResultWrapper.h"
#include "Data/FriendBlueprintWrapper.h"
#include "Data/MapData.h"
#include "AKZFGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSessionsRecievedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFriendsReadCompleteDelegate, bool, bWasSuccessful);

UCLASS(Blueprintable, BlueprintType)
class AKZFVROOMVROOM_API UAKZFGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UAKZFGameInstance();

	UFUNCTION(BlueprintCallable, Category="Maps")
	TArray<FMapInformation> LoadMaps();

	UFUNCTION(BlueprintCallable, Category="Friends")
	TArray<UFriendBlueprintWrapper*> GetFriendNames(int PlayerId);

	UPROPERTY(BlueprintReadOnly, Category="Online|Session")
	TArray<USessionSearchResultWrapper*> Results;
	
	UPROPERTY(BlueprintAssignable, Category="Online|Session")
	FSessionsRecievedDelegate SearchSessionsRecieved;

	UFUNCTION(BlueprintCallable, Category="Online|Session")
	void ExitSession();

	/* Begins hosting a session */
	UFUNCTION(BlueprintCallable, Category="Online|Session")
	bool HostSession( FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/* Create session delegate, will be bound to in the constructor to recieve created 
		session and forward onto OnCreate Sesssion which will then start the session.*/
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Start session delegate, should be initialized in the constructor and
		binds to the OnStartOnlineGameComplete method. */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/* Handles for our reigstered delegates. So precious. */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	/* Recieves the newly created session, if it was succesful this will then start the session. */
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	/* Recieves the started session, and if successful takes the player to the lobby with listen enabled */
	virtual void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/* Searches for available sessions */
	UFUNCTION(BlueprintCallable, Category="Online|Session")
	void FindSessions(FName SessionName, bool bIsLAN, bool bIsPrescense);

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnFindSessionsComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category="Online|Session")
	bool JoinSession(USessionSearchResultWrapper* Result);

	bool JoinSession(FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	virtual bool JoinSession(ULocalPlayer* player, int32 id) override;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnSessionDestroyCompleteDelegateHandle;

	/* Destroys the session */
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/* Initiates a request to read all the friends */
	UFUNCTION(BlueprintCallable,Category="Online|Friends")
	bool ReadFriendsList();
	/* Async delegates to be called when the friends list read is complete. */
	FOnReadFriendsListComplete OnReadFriendsListCompleteDelegate;
	FDelegateHandle OnReadFriendsListCompleteDelegateHandle;

	/* Called when the friends list read is complete */
	virtual void OnFriendsReadComplete(int32 number, bool bWasSuccessful, const FString& ListName, const FString& NotAClue);

	/* Called to signal to blueprints that the read is complete */
	UPROPERTY(BlueprintAssignable, Category="Online|Friends")
	FFriendsReadCompleteDelegate FriendsReadComplete;

	FOnSessionUserInviteAcceptedDelegate SessionUserInviteAcceptedDelegate;
	FDelegateHandle SessionUserInviteAcceptedDelegateHandle;

	virtual void SessionUserInviteAccepted(const bool bWasSuccessful, const int32 number, TSharedPtr<const FUniqueNetId> userId, const FOnlineSessionSearchResult& sessionResult);
};