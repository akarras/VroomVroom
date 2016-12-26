// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFLobbyController.h"

AAKZFLobbyController::AAKZFLobbyController()
{
	ConstructorHelpers::FObjectFinder<UBlueprint>WidgetClass(TEXT("WidgetBlueprint'/Game/VroomVroom/UI/Lobby.Lobby'"));
	if (WidgetClass.Succeeded())
	{
		MenuClass = WidgetClass.Object->GeneratedClass;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("No class :("));
	}
	//MenuClass = WidgetRef.Class;
}

void AAKZFLobbyController::BeginPlay()
{
	if (MenuClass)
	{
		Menu = CreateWidget<UUserWidget>(this, MenuClass);
		Menu->AddToViewport();
		bShowMouseCursor = true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Widget not loaded"));
	}
}