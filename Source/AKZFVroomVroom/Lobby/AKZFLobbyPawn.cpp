// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFLobbyPawn.h"


// Sets default values
AAKZFLobbyPawn::AAKZFLobbyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAKZFLobbyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAKZFLobbyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AAKZFLobbyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

