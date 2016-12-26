// Fill out your copyright notice in the Description page of Project Settings.

#include "AKZFVroomVroom.h"
#include "Blueprint/UserWidget.h"
#include "AKZFMenuController.h"

AAKZFMenuController::AAKZFMenuController()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> MenuWidget(TEXT("WidgetBlueprint'/Game/VroomVroom/UI/MainMenu.MainMenu'"));
	if (MenuWidget.Object) 
	{
		MenuClass = MenuWidget.Object->GeneratedClass;
	}
}

void AAKZFMenuController::BeginPlay()
{
	bShowMouseCursor = true;
	if (MenuClass)
	{
		MainMenu = CreateWidget<UUserWidget>(this, MenuClass);
		if (MainMenu)
		{
			MainMenu->AddToViewport();
		}
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(0, 10.0f, FColor(255, 0, 0), FString("Widget Class Not Loaded"));
	}
}
