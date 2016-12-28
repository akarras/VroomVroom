// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "AKZFVroomVroomPawn.h"
#include "AKZFRacePlayerState.h"
#include "AKZFRaceGameState.h"
#include "AKZFRaceCheckpoint.h"


// Sets default values
AAKZFRaceCheckpoint::AAKZFRaceCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	box = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	box->OnComponentBeginOverlap.AddDynamic(this, &AAKZFRaceCheckpoint::OnOverlapped);
	//RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root Scenecomponent"));
	box->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false));

	NextCheckpointNumber = CheckpointNumber + 1;
}

// Called when the game starts or when spawned
void AAKZFRaceCheckpoint::BeginPlay()
{
	Super::BeginPlay();
}

void AAKZFRaceCheckpoint::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Role == ROLE_Authority) 
	{
		AAKZFVroomVroomPawn* pawn = Cast<AAKZFVroomVroomPawn>(OtherActor);
		if (pawn)
		{
			// Get the player's state
			AAKZFRacePlayerState* state = Cast<AAKZFRacePlayerState>(pawn->PlayerState);
			int nextCheckpoint = state->NextCheckpoint; // Get the player's next checkpoint
			if (CheckpointNumber == nextCheckpoint) // Check if this is the checkpoint they're going for
			{
				// If so set their next checkpoint to our next checkpoint
				state->NextCheckpoint = NextCheckpointNumber;
				state->LastCheckpoint = CheckpointNumber;
				if (IsLapFinisher) // Check if this is the final goal
				{
					state->LapsComplete++; // Increment laps complete
					// Check if the player has finished all the laps!
					AGameStateBase* game = UGameplayStatics::GetGameState(this);
					AAKZFRaceGameState* raceState = Cast<AAKZFRaceGameState>(game);
					if (raceState)
					{
						state->HasFinished = state->LapsComplete >= raceState->NumberOfLaps;
					}
				}
				GEngine->AddOnScreenDebugMessage(1, 200.0f, FColor(255, 0, 0), FString("Checkpoint Number ") + FString::FormatAsNumber(CheckpointNumber));
			}
		}
	}
}

// Called every frame
void AAKZFRaceCheckpoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

