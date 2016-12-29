// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "Data/SessionSearchResultWrapper.h"
#include "AKZFGameInstance.h"

UAKZFGameInstance::UAKZFGameInstance()
{
	// Creation delegates
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UAKZFGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UAKZFGameInstance::OnStartOnlineGameComplete);
	// Find session delegates
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UAKZFGameInstance::OnFindSessionsComplete);
	// Destroy Session Delegate
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UAKZFGameInstance::OnDestroySessionComplete);
	// Join Session Delegate
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UAKZFGameInstance::OnJoinSessionComplete);
	// Read friends list delegate
	OnReadFriendsListCompleteDelegate = FOnReadFriendsListComplete::CreateUObject(this, &UAKZFGameInstance::OnFriendsReadComplete);
}

TArray<FMapInformation> UAKZFGameInstance::LoadMaps()
{
	FStringAssetReference MapReference(FString("MapData'/Game/VroomVroom/Data/Maps.Maps'"));
	UMapData* asset = Cast<UMapData>(MapReference.TryLoad());
	return asset->Maps;
}

bool UAKZFGameInstance::HostSession(FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	// Grab our default online subsystem
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the session interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Manufacture our session settings. MakeShareable makes a TSharedPtr, which is Unreal's shared_ptr.
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence; // According to a stranger on the forums, false presence = Game Server, true presence = New session p2p API
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			SessionSettings->Set(SETTING_MAPNAME, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			
			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be succesful!")
			return Sessions->CreateSession(0, SessionName, *SessionSettings);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("No session subsystem"));
		}

	}

	return false;
}

void UAKZFGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the session interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear SessionComplete handle
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate
				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UAKZFGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so that we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}
	// If the start was successful, open ze map!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true, "listen");
	}
}

void UAKZFGameInstance::FindSessions(FName SessionName, bool bIsLAN, bool bIsPresence)
{
	// Get our default subsystem
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Grab our default session interface.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;
			// Only search presence if bIsPresence is set to true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the delegate handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally search!
			Sessions->FindSessions(0, SearchSettingsRef);
		}
	}
	else
	{
		// If it dun work just say false on the sessions complete
		OnFindSessionsComplete(false);
	}
}

void UAKZFGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get sessions
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear delegate handle since we finished call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			Results.Empty(SessionSearch->SearchResults.Num());
			TArray<FOnlineSessionSearchResult> results = SessionSearch->SearchResults;
			for (FOnlineSessionSearchResult Result : results)
			{
				USessionSearchResultWrapper* wrapper = NewObject<USessionSearchResultWrapper>(USessionSearchResultWrapper::StaticClass());
				wrapper->SessionResult = Result;
				Results.Add(wrapper);
			}
			if (SearchSessionsRecieved.IsBound())
			{
				SearchSessionsRecieved.Broadcast();
			}
			//SearchSessionsRecieved.Broadcast(Results);
		}
	}
}

bool UAKZFGameInstance::JoinSession(USessionSearchResultWrapper * Result)
{
	// Simply calls the C++ version of this. This function serves as a way to call it from our UMG widget.
	return JoinSession(GameSessionName, Result->SessionResult);
}

bool UAKZFGameInstance::JoinSession(FName SessionName, const FOnlineSessionSearchResult & SearchResult)
{
	bool bSuccessful = false;
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Attempting to join session"));
	// Get our online subsystem
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Gets a reference to our sessions interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Set our delegate handle for joining the session
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			// Attempt to join the given session.
			bSuccessful = Sessions->JoinSession(0, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UAKZFGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));
	// Get our online subsystem
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Gets a reference to our sessions interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Make sure we clear the delegate that brought us here
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
			// Grab the first player controller in the session
			APlayerController* const PlayerController = GetFirstLocalPlayerController();
			FString TravelURL;
			// Gets a connection string to our other player.
			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				// Instruct the client to travel to the given url for the session.
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}

}

void UAKZFGameInstance::ExitSession()
{
	// Get our online subsystem
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get a reference to our session interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Set our delegate handle
			OnSessionDestroyCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			// Tell the session interface to destroy the current session.
			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UAKZFGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s %d"), *SessionName.ToString(), static_cast<int32>(bWasSuccessful)));
	// Get our online subsystem
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get a reference to the sessions interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate handle for destroyin the session because we're done with it now
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnSessionDestroyCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// If we successfully destroy the session we should go back to our main menu
				UGameplayStatics::OpenLevel(GetWorld(), "Menu", true);
			}
		}
	}
}

bool UAKZFGameInstance::ReadFriendsList()
{
	bool Success = false;
	// Get the default online subsystem
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Gets a reference to our friends interface
		IOnlineFriendsPtr Friends = OnlineSub->GetFriendsInterface();
		if (Friends.IsValid())
		{
			// Starts to read the list, must later be read with GetFriendsList.
			Success = Friends->ReadFriendsList(0, FString(""), OnReadFriendsListCompleteDelegate);
		}
	}
	return Success;
}

void UAKZFGameInstance::OnFriendsReadComplete(int32 number, bool bWasSuccessful, const FString & ListName, const FString & NotAClue)
{
	// Pass this onto our blueprint delegate
	FriendsReadComplete.Broadcast(bWasSuccessful);
}

TArray<FString> UAKZFGameInstance::GetFriendNames(int playerSlot)
{
	UE_LOG(LogActor, Warning, TEXT("Trying to get friend names"));
	TArray<TSharedRef<FOnlineFriend>> FriendsList;
	TArray<FString> FriendNames;

	// Get our default subsystem
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Gets our friend interface
		IOnlineFriendsPtr Friends = OnlineSub->GetFriendsInterface();
		if (Friends.IsValid())
		{
			// Tries to get our friend list
			if (Friends->GetFriendsList(playerSlot, FString(""), FriendsList))
			{
				for (TSharedPtr<FOnlineFriend> Friend : FriendsList)
				{
					// Iterates through the given friends and adds them to the friend names array
					FriendNames.Add(Friend->GetDisplayName());
					//@todo Make a wrapper so that we can expose friends to blueprints. Man that sounds dirty.
				}
			}
			else
			{
				UE_LOG(LogActor, Warning, TEXT("Error getting friends!"));
			}
		}
	}
	return FriendNames; // Returns our friends!
}
