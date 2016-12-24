// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "AKZFLobbyController.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API AAKZFLobbyController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AAKZFLobbyController();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<UObject> MenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	UUserWidget* Menu;
	
	
	
};
