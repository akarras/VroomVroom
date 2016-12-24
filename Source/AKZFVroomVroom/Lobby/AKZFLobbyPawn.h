// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/Pawn.h"
#include "AKZFLobbyPawn.generated.h"

UCLASS()
class AKZFVROOMVROOM_API AAKZFLobbyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAKZFLobbyPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
