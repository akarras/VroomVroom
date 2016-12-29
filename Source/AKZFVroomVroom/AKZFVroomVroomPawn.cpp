// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AKZFVroomVroom.h"
#include "AKZFVroomVroomPawn.h"
#include "AKZFVroomVroomWheelFront.h"
#include "AKZFVroomVroomWheelRear.h"
#include "AKZFVroomVroomHud.h"
#include "AKZFRacePlayerState.h"
#include "AKZFRaceCheckpoint.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"


// Needed for VR Headset
#include "Engine.h"
#if HMD_MODULE_INCLUDED
#include "IHeadMountedDisplay.h"
#endif // HMD_MODULE_INCLUDED

const FName AAKZFVroomVroomPawn::LookUpBinding("LookUp");
const FName AAKZFVroomVroomPawn::LookRightBinding("LookRight");
const FName AAKZFVroomVroomPawn::EngineAudioRPM("RPM");

#define LOCTEXT_NAMESPACE "VehiclePawn"

AAKZFVroomVroomPawn::AAKZFVroomVroomPawn()
{
	// Setup can move
	bCanMove = false;

	// Setup Trace Params
	traceParams.bTraceComplex = true;
	traceParams.bTraceAsyncScene = true;
	traceParams.bReturnPhysicalMaterial = true;
	traceParams.AddIgnoredActor(this);

	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/VehicleAdv/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);
	
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/VehicleAdv/Vehicle/VehicleAnimationBlueprint"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Setup friction materials
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/Slippery.Slippery"));
	SlipperyMaterial = SlipperyMat.Object;
		
	static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> NonSlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/NonSlippery.NonSlippery"));
	NonSlipperyMaterial = NonSlipperyMat.Object;

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	// Wheels/Tyres
	// Setup the wheels
	Vehicle4W->WheelSetups[0].WheelClass = UAKZFVroomVroomWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("PhysWheel_FL");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UAKZFVroomVroomWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("PhysWheel_FR");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UAKZFVroomVroomWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("PhysWheel_BL");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UAKZFVroomVroomWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("PhysWheel_BR");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	// Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	// Engine 
	// Torque setup
	Vehicle4W->MaxEngineRPM = 5700.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);
 
	// Adjust the steering 
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);
	
	// TODO We should figure out how to make it so we don't need this
	Vehicle4W->bDeprecatedSpringOffsetMode = true;


 	// Transmission	
	// We want 4wd
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	
	// Drive the front wheels a little more than the rear
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

	// Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	// Physics settings
	// Adjust the center of mass - the buggy is quite low
	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.0f, 0.0f, 0.0f);
	}

	// Set the inertia scale. This controls how the mass of the vehicle is distributed.
	Vehicle4W->InertiaTensorScale = FVector(1.0f, 1.333f, 1.2f);

	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
	SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 125.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create the chase camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(-125.0, 0.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(-34.0f, -10.0f, 50.0f);
	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	// In car HUD
	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	InCarSpeed->SetRelativeLocation(FVector(35.0f, -6.0f, 20.0f));
	InCarSpeed->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	InCarSpeed->SetupAttachment(GetMesh());

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	InCarGear->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
	InCarGear->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	InCarGear->SetupAttachment(GetMesh());
	
	// Setup the audio component and allocate it a sound cue
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_Cue.Engine_Loop_Cue"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(GetMesh());

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bIsLowFriction = false;
	bInReverseGear = false;
	
}

void AAKZFVroomVroomPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAKZFVroomVroomPawn, bCanMove);
}

void AAKZFVroomVroomPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAKZFVroomVroomPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAKZFVroomVroomPawn::MoveRight);
	PlayerInputComponent->BindAxis(LookUpBinding);
	PlayerInputComponent->BindAxis(LookRightBinding);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AAKZFVroomVroomPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AAKZFVroomVroomPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AAKZFVroomVroomPawn::OnToggleCamera);
	PlayerInputComponent->BindAction("Respawn", IE_Pressed, this, &AAKZFVroomVroomPawn::OnRespawn);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAKZFVroomVroomPawn::OnResetVR); 
}

void AAKZFVroomVroomPawn::MoveForward(float Val)
{
	if (bCanMove) // Todo, find a way of making it so the engine sounds still play even though we're not moving.
	{
		if (isGrounded()) {
			GetVehicleMovementComponent()->SetThrottleInput(Val);
		}
		else {
			GetVehicleMovementComponent()->SetThrottleInput(0);
			MoveForwardServer(Val);
		}
	}
}

void AAKZFVroomVroomPawn::MoveForwardServer_Implementation(float Val) {
	FTransform t = GetVehicleMovementComponent()->UpdatedPrimitive->GetRelativeTransform();
	FQuat rot = t.GetRotation();
	FVector v;
	v.X = 0;
	v.Y = Val * 3.5;
	v.Z = 0;
	GetVehicleMovementComponent()->UpdatedPrimitive->SetPhysicsAngularVelocity(t.TransformVector(v), true);
}

bool AAKZFVroomVroomPawn::MoveForwardServer_Validate(float Val) {
	return true;
}

void AAKZFVroomVroomPawn::MoveRight(float Val)
{
	if (isGrounded()) {
		GetVehicleMovementComponent()->SetSteeringInput(Val);
	}
	else {
		GetVehicleMovementComponent()->SetSteeringInput(0);
		MoveRightServer(Val);
	}
}

void AAKZFVroomVroomPawn::MoveRightServer_Implementation(float Val) {
	FTransform t = GetVehicleMovementComponent()->UpdatedPrimitive->GetRelativeTransform();
	FQuat rot = t.GetRotation();
	FVector v;
	v.X = Val * -3.5;
	v.Y = 0;
	v.Z = 0;
	GetVehicleMovementComponent()->UpdatedPrimitive->SetPhysicsAngularVelocity(t.TransformVector(v), true);
}

bool AAKZFVroomVroomPawn::MoveRightServer_Validate(float Val) {
	return true;
}

void AAKZFVroomVroomPawn::OnRespawn() 
{
	// Pretty much just tell the server we want to respawn when we press the button
	RespawnServer();
}

void AAKZFVroomVroomPawn::RespawnServer_Implementation()
{
	// Handle respawning!
	AAKZFRacePlayerState* state = Cast<AAKZFRacePlayerState>(PlayerState);
	// Make sure the player's not already done
	if (!state->HasFinished) 
	{
		// Get the previous checkpoint the player was at.
		int lastCheckpoint = state->LastCheckpoint;
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, "Respawning to checkpoint " + FString::FromInt(lastCheckpoint));
		// Get the actor for the checkpoint
		TSubclassOf<AAKZFRaceCheckpoint> ClassToFind;
		for (TObjectIterator<AAKZFRaceCheckpoint> Checkpoints; Checkpoints; ++Checkpoints)
		{
			AAKZFRaceCheckpoint* Checkpoint = *Checkpoints;
			GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Magenta, FString::Printf(TEXT("Checkpoint #%d"), Checkpoint->CheckpointNumber));
			if (Checkpoint->CheckpointNumber == lastCheckpoint)
			{
				// If the checkpoint we're checking is the checkpoint we're trying to go to, WE SHOULD GO THERE DAMNIT!
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("Going to checkpoint!"));
				SetActorTransform(Checkpoint->GetTransform(), false, nullptr, ETeleportType::TeleportPhysics);
				GetMesh()->SetPhysicsLinearVelocity(FVector(0, 0, 0));
				GetMesh()->SetPhysicsAngularVelocity(FVector(0, 0, 0));
				return;
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Magenta, FString::Printf(TEXT("Checkpoint %d"), Actors.Num()));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Checkpoint not found"));
}

bool AAKZFVroomVroomPawn::RespawnServer_Validate()
{
	return true; // ITS A BUTTON? I guess we could make sure they're not spamming. Still stupid.
}

void AAKZFVroomVroomPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AAKZFVroomVroomPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AAKZFVroomVroomPawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void AAKZFVroomVroomPawn::EnableIncarView(const bool bState)
{
	if (bState != bInCarCameraActive)
	{
		bInCarCameraActive = bState;
		
		if (bState == true)
		{
			OnResetVR();
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}
		
		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}

void AAKZFVroomVroomPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update phsyics material
	UpdatePhysicsMaterial();

	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar hud
	SetupInCarHUD();

	bool bHMDActive = false;
#if HMD_MODULE_INCLUDED
	if ((GEngine->HMDDevice.IsValid() == true ) && ( (GEngine->HMDDevice->IsHeadTrackingAllowed() == true) || (GEngine->IsStereoscopic3D() == true)))
	{
		bHMDActive = true;
	}
#endif // HMD_MODULE_INCLUDED
	if( bHMDActive == false )
	{
		if ( (InputComponent) && (bInCarCameraActive == true ))
		{
			FRotator HeadRotation = InternalCamera->RelativeRotation;
			HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
			HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
			InternalCamera->RelativeRotation = HeadRotation;
		}
	}	

	// Pass the engine RPM to the sound component
	float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed()*RPMToAudioScale);
}

void AAKZFVroomVroomPawn::BeginPlay()
{
	Super::BeginPlay();

	bool bWantInCar = false;
	// First disable both speed/gear displays 
	bInCarCameraActive = false;
	InCarSpeed->SetVisibility(bInCarCameraActive);
	InCarGear->SetVisibility(bInCarCameraActive);

	// Enable in car view if HMD is attached
#if HMD_MODULE_INCLUDED
	bWantInCar = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
#endif // HMD_MODULE_INCLUDED

	EnableIncarView(bWantInCar);
	// Start an engine sound playing
	EngineSoundComponent->Play();
}

void AAKZFVroomVroomPawn::OnResetVR()
{
#if HMD_MODULE_INCLUDED
	if (GEngine->HMDDevice.IsValid())
	{
		GEngine->HMDDevice->ResetOrientationAndPosition();
		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
		GetController()->SetControlRotation(FRotator());
	}
#endif // HMD_MODULE_INCLUDED
}

void AAKZFVroomVroomPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);
	int32 Gear = GetVehicleMovement()->GetCurrentGear();

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));


	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}

}

void AAKZFVroomVroomPawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr))
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);
		
		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

void AAKZFVroomVroomPawn::UpdatePhysicsMaterial()
{
	if (GetActorUpVector().Z < 0)
	{
		if (bIsLowFriction == true)
		{
			GetMesh()->SetPhysMaterialOverride(NonSlipperyMaterial);
			bIsLowFriction = false;
		}
		else
		{
			GetMesh()->SetPhysMaterialOverride(SlipperyMaterial);
			bIsLowFriction = true;
		}
	}
}

bool AAKZFVroomVroomPawn::isGrounded() {
	FVector downV = GetActorUpVector() * -1;
	FVector beginLoc = GetActorLocation();
	FVector endLoc = (downV * 50) + GetActorLocation();
	//DrawDebugLine(GetWorld(), beginLoc, endLoc, FColor::Red, false, -1.0f, (uint8)'\000', 5.0f);
	FHitResult OutHit;
	GetWorld()->SweepSingleByChannel(OutHit, beginLoc, endLoc, FQuat(), ECC_Visibility, FCollisionShape::MakeSphere(10), traceParams);

	if (OutHit.bBlockingHit) {
		if (OutHit.GetActor()->ActorHasTag("Ground")) {
			return true;
		}
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
