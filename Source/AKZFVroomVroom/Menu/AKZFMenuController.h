// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "AKZFMenuController.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API AAKZFMenuController : public APlayerController
{
	GENERATED_BODY()
public:
	AAKZFMenuController();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UObject> MenuClass;

	UPROPERTY(BlueprintReadWrite, Category="Widgets")
	UUserWidget* MainMenu;
	
};
