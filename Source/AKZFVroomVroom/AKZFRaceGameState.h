// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/GameStateBase.h"
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

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Laps")
	int NumberOfLaps;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Race State")
	ERaceState RaceState;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Race State")
	float StateRoundLimit;

	void AdvanceState();
	FTimerHandle RoundTimerHandle;
	void RoundTimerUp();

	void DoReset();
};
