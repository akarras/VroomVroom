// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "SessionSearchResultWrapper.h"

USessionSearchResultWrapper::USessionSearchResultWrapper()
{

}

USessionSearchResultWrapper::USessionSearchResultWrapper(FOnlineSessionSearchResult Session)
{
	SessionResult = Session;
	
}

FString USessionSearchResultWrapper::GetOwnerName()
{
	return SessionResult.Session.OwningUserName;
}

FString USessionSearchResultWrapper::GetSessionID()
{
	return SessionResult.Session.GetSessionIdStr();
}
