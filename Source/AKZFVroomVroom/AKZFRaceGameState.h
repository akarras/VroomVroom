// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/GameStateBase.h"
#include "Data/MapData.h"
#include "AKZFRaceGameState.generated.h"

UENUM(BlueprintType)
enum class ERaceState : uint8
{
	R_Warmup UMETA(DisplayName = "Warmup"),
	R_Started UMETA(DisplayName = "Started"),
	R_TimeToFinish UMETA(DisplayName = "Finish Time"),
	R_Finished UMETA(DisplayName = "Finished")
};

UCLASS()
class AKZFVROOMVROOM_API AAKZFRaceGameState : public AGameStateBase
{
	GENERATED_BODY()
private:
	void UpdateRoundTime(float time);
public:
	AAKZFRaceGameState();

	virtual void BeginPlay() override;

	/* Number of laps in the game state. */
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Laps")
	int NumberOfLaps;

	/* Current RaceState. Should only be assigned from AdvanceState. Really probably should make this a get only function. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Race State")
	ERaceState RaceState;
	/* Current time limit. Should only be assigned from AdvanceState. Might make this into a get only function later. */
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Race State")
	float StateRoundLimit;
	/* Advances the state to the next state with the proper time */
	void AdvanceState();
	/* Handle for the timer */
	FTimerHandle RoundTimerHandle;
	/* Called when the timer handle is over */
	void RoundTimerUp();
	/* Starts the race */
	void StartRace();
	/* Show end vote menu */
	void RaceOver();
	/* Travels to the map with the most votes */
	void TravelNextMap();

	/* Tallies votes from player states*/
	TMap<FString, int> GetTalliedMapVotes();
};
