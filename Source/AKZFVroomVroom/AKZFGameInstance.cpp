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
}

bool UAKZFGameInstance::HostSession(FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the session interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence; // According to a stranger on the forums, false presence = Game Server, true presence = New session p2p API
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bShouldAdvertise = true;
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
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("No subsystem"));
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
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		
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
				USessionSearchResultWrapper* wrapper = ConstructObject<USessionSearchResultWrapper>(USessionSearchResultWrapper::StaticClass());
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
	return JoinSession(GameSessionName, Result->SessionResult);
}

bool UAKZFGameInstance::JoinSession(FName SessionName, const FOnlineSessionSearchResult & SearchResult)
{
	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bSuccessful = Sessions->JoinSession(0, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UAKZFGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		
		if (Sessions.IsValid())
		{
			// Make sure we clear the delegate that brought us here
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			APlayerController* const PlayerController = GetFirstLocalPlayerController();
			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}

}

void UAKZFGameInstance::ExitSession()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			OnSessionDestroyCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UAKZFGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s %d"), *SessionName.ToString(), static_cast<int32>(bWasSuccessful)));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnSessionDestroyCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "Menu", true);
			}
		}
	}
}

TArray<FString> UAKZFGameInstance::GetFriendNames(int playerSlot)
{
	UE_LOG(LogActor, Warning, TEXT("Trying to get friend names"));
	TArray<TSharedRef<FOnlineFriend>> FriendsList;
	TArray<FString> FriendNames;
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get(FName("Steam"));
	
	if (OnlineSub)
	{
		IOnlineFriendsPtr Friends = OnlineSub->GetFriendsInterface();
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		Identity->AutoLogin(0);
		if (Friends.IsValid())
		{
			if (Friends->GetFriendsList(playerSlot, FString(""), FriendsList))
			{
				for (TSharedPtr<FOnlineFriend> Friend : FriendsList)
				{
					FriendNames.Add(Friend->GetDisplayName());
					UE_LOG(LogActor, Warning, TEXT("Logged: %s"), *Friend->GetDisplayName());
				}
			}
			else
			{
				UE_LOG(LogActor, Warning, TEXT("Error getting friends!"));
			}
		}
	}
	return FriendNames;
}
