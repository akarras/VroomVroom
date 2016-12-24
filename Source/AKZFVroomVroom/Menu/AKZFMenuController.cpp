// Fill out your copyright notice in the Description page of Project Settings.

#include "AKZFVroomVroom.h"
#include "Blueprint/UserWidget.h"
#include "AKZFMenuController.h"

AAKZFMenuController::AAKZFMenuController()
{
	FStringAssetReference MenuWidget(FString("WidgetBlueprint'/Game/VroomVroom/UI/MainMenu.MainMenu'"));
	UObject* menuObject = MenuWidget.TryLoad();
	if (menuObject)
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(0, 255, 0), menuObject->GetClass()->GetName());
		MenuClass = menuObject->GetClass();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Widget Class Not Loaded"));
	}
}

void AAKZFMenuController::BeginPlay()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget(TEXT("WidgetBlueprint'/Game/VroomVroom/UI/MainMenu.MainMenu'"));
	
	/*if (menuObject)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor(0, 255, 0), menuObject->GetClass()->GetName());
		
		MainMenu = CreateWidget<UUserWidget>(this, menuObject->GetClass());
		//MainMenu->AddToViewport();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(2, 10.0f, FColor(255, 250, 0), FString("Widget Class Not Loaded"));
	}*/


	bShowMouseCursor = true;
	if (MenuClass)
	{
		
		MainMenu = CreateWidget<UUserWidget>(this, MenuClass);
		MainMenu->AddToViewport();
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Widget Class Not Loaded"));
	}
}
