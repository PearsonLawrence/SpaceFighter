// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceShooterPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "UnrealNetwork.h"


ASpaceShooterPawn::ASpaceShooterPawn()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = PlaneMesh;


	//TODO: add third person
	////////////////////////////////////////////////////////////////////////
	//// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 0; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f,0.f,0.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 0.f;
	////////////////////////////////////////////////////////////////////////

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 100;
	CurrentForwardSpeed = 500.f;
}

void ASpaceShooterPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ASpaceShooterPawn, Health);
}

void ASpaceShooterPawn::Tick(float DeltaSeconds)
{
	
		ticktime -= GetWorld()->GetDeltaSeconds();

		if (Role == ROLE_Authority)
		{
			UpdateLocationFromServer(ShipInput, DeltaSeconds);
		}
	


	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}

void ASpaceShooterPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void ASpaceShooterPawn::UpdatePitch(float Val)
{
	ShipInput.PitchInput = Val;
}
void ASpaceShooterPawn::UpdateYaw(float Val)
{
	ShipInput.YawInput = Val;
}
void ASpaceShooterPawn::UpdateRoll(float Val)
{
	ShipInput.RollInput = Val;
}
void ASpaceShooterPawn::UpdateThrust(float Val)
{
	ShipInput.ThrustInput = Val;
}
void ASpaceShooterPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this,&ASpaceShooterPawn::UpdateThrust);
	PlayerInputComponent->BindAxis("Pitch", this, &ASpaceShooterPawn::UpdatePitch);
	PlayerInputComponent->BindAxis("Yaw", this, &ASpaceShooterPawn::UpdateYaw);
	PlayerInputComponent->BindAxis("Roll", this, &ASpaceShooterPawn::UpdateRoll);
}

void ASpaceShooterPawn::ThrustInput(float Val)
{
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}


bool ASpaceShooterPawn::UpdateLocationFromServer_Validate(FInputStruct Input, float DeltaSeconds)
{

	return true;

}

void ASpaceShooterPawn::UpdateLocationFromServer_Implementation(FInputStruct Input, float DeltaSeconds)
{
	FString YawS = FString::SanitizeFloat(Input.YawInput);
	FString RollS = FString::SanitizeFloat(Input.RollInput);
	FString PitchS = FString::SanitizeFloat(Input.PitchInput);
	FString ThrustS = FString::SanitizeFloat(Input.ThrustInput);

	UE_LOG(LogTemp, Warning, TEXT("Yaw %s"), *YawS);
	UE_LOG(LogTemp, Warning, TEXT("Roll %s"), *RollS);
	UE_LOG(LogTemp, Warning, TEXT("Pitch %s"), *PitchS);
	UE_LOG(LogTemp, Warning, TEXT("Thrust %s"), *ThrustS);

	const bool bIsTurning = FMath::Abs(Input.YawInput) > 0.2f;

	bool bHasInput = !FMath::IsNearlyEqual(Input.ThrustInput, 0.f);


	float TargetPitchSpeed = (Input.PitchInput * TurnSpeed * -1.f);
	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, DeltaSeconds, 2.f);

	
	float TargetRollSpeed = (Input.RollInput * TurnSpeed * -1.f);

	// Smoothly interpolate to target pitch speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, DeltaSeconds, 2.f);

	float TargetYawSpeed = (Input.YawInput * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, DeltaSeconds, 2.f);

	// Is there any left/right input?

	float CurrentAcc = bHasInput ? (Input.ThrustInput * Acceleration) : (-0.5f * Acceleration);
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);

	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);

	// Move plane forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);
	
	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
}

void ASpaceShooterPawn::Roll(float Val)
{
	// Target pitch speed is based in input
	float TargetRollSpeed = (Val * TurnSpeed * -1.f);

	// Smoothly interpolate to target pitch speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ASpaceShooterPawn::Yaw(float Val)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

}
