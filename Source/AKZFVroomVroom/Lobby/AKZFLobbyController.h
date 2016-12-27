// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Data/MapData.h"
#include "AKZFLobbyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCurrentMapDelegate, float, data);

UCLASS()
class AKZFVROOMVROOM_API AAKZFLobbyController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AAKZFLobbyController();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	UUserWidget* Menu;

	UPROPERTY(BlueprintAssignable, Category = "Test")
	FCurrentMapDelegate OnMapUpdated;
	
};
