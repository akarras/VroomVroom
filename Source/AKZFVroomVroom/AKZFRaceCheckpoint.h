// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/Actor.h"
#include "AKZFRaceCheckpoint.generated.h"

UCLASS()
class AKZFVROOMVROOM_API AAKZFRaceCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAKZFRaceCheckpoint();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoint")
	int CheckpointNumber;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoint")
	int NextCheckpointNumber;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoint")
	bool IsLapFinisher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
	UBoxComponent* box;
};
