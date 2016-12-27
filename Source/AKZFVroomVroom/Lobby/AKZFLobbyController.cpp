// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFLobbyController.h"

AAKZFLobbyController::AAKZFLobbyController()
{

}

void AAKZFLobbyController::BeginPlay()
{
	if (MenuClass)
	{
		Menu = CreateWidget<UUserWidget>(this, MenuClass);
		if (Menu) 
		{
			Menu->AddToViewport();
		}
		bShowMouseCursor = true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Widget not loaded"));
	}
}