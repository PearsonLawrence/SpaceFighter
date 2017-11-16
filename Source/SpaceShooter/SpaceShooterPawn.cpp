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
#include "Kismet/GameplayStatics.h"
#include "Cannon.h"
#include "BaseProjectile.h"
#include "Components/PrimitiveComponent.h"
#include "ShipStatistics.h"
#include "DrawDebugHelpers.h"

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

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 0; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 0.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller



	Cannon2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon2"));		// Only the owning player wil
	Cannon2->SetupAttachment(PlaneMesh, TEXT("CannonRight"));

	Cannon1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon1"));		// Only the owning player wil
	Cannon1->SetupAttachment(PlaneMesh, TEXT("CannonLeft"));



	ShootPointLeft = CreateDefaultSubobject<USceneComponent>(TEXT("ShootLeft"));
	ShootPointLeft->SetupAttachment(PlaneMesh, TEXT("LeftShootPoint"));


	ShootPointRight = CreateDefaultSubobject<USceneComponent>(TEXT("ShootRight"));
	ShootPointRight->SetupAttachment(PlaneMesh, TEXT("RightShootPoint"));

	Stats = CreateDefaultSubobject<UShipStatistics>(TEXT("ShipStatisticsComponent"));
	Stats->Health = 100;
	Stats->Energy = 100;


	// Set handling parameters
	Acceleration = 2000;
	TurnSpeed = 100;
	HoverTurnSpeed = 150;
	MaxSpeed = 7000.f;
	MinSpeed = 2000.f;
	CurrentForwardSpeed = 500.f;

	HoverAcceleration = 200.f;

	HoverMaxSpeed = 50;
	HoverMinSpeed = -50;
	HoverDrag = 3.f;
	ShootDelay = .05f;
	RotateSpeed = 1.f;
	
	bIsHovering = false;
}

void ASpaceShooterPawn::FlyMovement(float DeltaSeconds)
{
	//const FVector WorldMove = FVector(0, CurrentRightSpeed * DeltaSeconds, 0);
	const FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds, 0, 0);


	FVector PrevPos = GetActorLocation();
	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	
	//AddActorWorldOffset(WorldMove, true);
	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;


	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
}

void ASpaceShooterPawn::HoverMovement(float DeltaSeconds)
{
	

	HoverMove.X += ForwardVal * DeltaSeconds;
	HoverMove.Y += RightVal * DeltaSeconds;

	

	HoverMove.X = (ForwardVal == 0) ? FMath::FInterpTo(HoverMove.X, 0, DeltaSeconds, HoverDrag) : FMath::Clamp(HoverMove.X,HoverMinSpeed,HoverMaxSpeed);
	HoverMove.Y = (RightVal == 0) ? FMath::FInterpTo(HoverMove.Y, 0, DeltaSeconds, HoverDrag) : FMath::Clamp(HoverMove.Y, HoverMinSpeed, HoverMaxSpeed);

	FVector PrevPos = GetActorLocation();
	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(HoverMove, true);

	
	


	//AddActorWorldOffset(WorldMove, true);
	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;


	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
}

void ASpaceShooterPawn::ToggleHover()
{
	UE_LOG(LogTemp, Warning, TEXT("toggle"));

	bIsHovering = (bIsHovering) ? false : true;
}


void ASpaceShooterPawn::Tick(float DeltaSeconds)
{
	if (bIsHovering)
	{
		HoverMovement(DeltaSeconds);
	}
	else
	{
		FlyMovement(DeltaSeconds);
	}

	Super::Tick(DeltaSeconds);
}

void ASpaceShooterPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}


void ASpaceShooterPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAction("ToggleHover", IE_Pressed, this, &ASpaceShooterPawn::ToggleHover);
	PlayerInputComponent->BindAxis("Thrust", this, &ASpaceShooterPawn::ThrustInput);
	PlayerInputComponent->BindAxis("Pitch", this, &ASpaceShooterPawn::Pitch);
	PlayerInputComponent->BindAxis("Roll", this, &ASpaceShooterPawn::Roll);
	PlayerInputComponent->BindAxis("Yaw", this, &ASpaceShooterPawn::Yaw);
	PlayerInputComponent->BindAxis("HoverForward", this, &ASpaceShooterPawn::HoverForwardInput);
	PlayerInputComponent->BindAxis("HoverRight", this, &ASpaceShooterPawn::HoverRightInput);
	PlayerInputComponent->BindAxis("HoverYaw", this, &ASpaceShooterPawn::HoverYaw);
	PlayerInputComponent->BindAxis("HoverPitch", this, &ASpaceShooterPawn::HoverPitch);
	PlayerInputComponent->BindAxis("HoverRoll", this, &ASpaceShooterPawn::HoverRoll);
	PlayerInputComponent->BindAxis("Fire", this, &ASpaceShooterPawn::Fire);
}


void ASpaceShooterPawn::Fire(float Val)
{
	setShootDelay -= GetWorld()->GetDeltaSeconds();
	if (Val >= .09)
	{

		if (setShootDelay <= 0)
		{
			setShootDelay = ShootDelay;
			
			FString text = FString::SanitizeFloat(setShootDelay);
			// try and fire a projectile
			if (ProjectileClass != NULL)
			{

				UWorld* const World = GetWorld();
				if (World != NULL)
				{
				
					const FRotator SpawnRotation = ShootPointLeft->GetComponentRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation = ((ShootPointLeft != nullptr) ? ShootPointLeft->GetComponentLocation() : ShootPointLeft->GetComponentLocation());

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						// spawn the projectile at the muzzle
						World->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					
						const FRotator SpawnRotation2 = ShootPointRight->GetComponentRotation();
						// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
						const FVector SpawnLocation2 = ((ShootPointRight != nullptr) ? ShootPointRight->GetComponentLocation() : GetActorLocation());

					
						// spawn the projectile at the muzzle
						World->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation2, SpawnRotation2, ActorSpawnParams);

				}
			}

		}
	}
}

void ASpaceShooterPawn::ThrustInput(float Val)
{
	if (!bIsHovering)
	{
		// Is there any input?
		bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
		// If input is not held down, reduce speed

		ThrustAcc += Val * (GetWorld()->GetDeltaSeconds() * Acceleration);
		// Calculate new speed
		float NewForwardSpeed = CurrentForwardSpeed + ThrustAcc;
		// Clamp between MinSpeed and MaxSpeed
		ThrustAcc = FMath::Clamp(ThrustAcc, MinSpeed, MaxSpeed);
		CurrentForwardSpeed = ThrustAcc;
	}
}

void ASpaceShooterPawn::Pitch(float Val)
{
	
	if (!bIsHovering)
	{
		// Target pitch speed is based in input
		float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

		//// When steering, we decrease pitch slightly
		TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

		//// Smoothly interpolate to target pitch speed
		CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	}
}

void ASpaceShooterPawn::Yaw(float Val)
{
	if (!bIsHovering)
	{
		// Target yaw speed is based on input
		float TargetYawSpeed = (Val * TurnSpeed);

		// Smoothly interpolate to target yaw speed
		CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

		// Is there any left/right input?
		const bool bIsTurning = FMath::Abs(Val) > 0.2f;
	}

}

void ASpaceShooterPawn::Roll(float Val)
{
	if (!bIsHovering)
	{
		// If turning, yaw value is used to influence roll
		// If not turning, rol to reverse current roll value.
		float TargetRollSpeed = (Val * TurnSpeed * -1.f);

		TargetRollSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

		//// Smoothly interpolate roll speed
		CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	}
}

void ASpaceShooterPawn::HoverForwardInput(float Val)
{
	ForwardVal = Val * HoverAcceleration;
}
void ASpaceShooterPawn::HoverRightInput(float Val)
{
	RightVal = Val * HoverAcceleration;
}
void ASpaceShooterPawn::HoverPitch(float Val)
{
	if (bIsHovering)
	{
		// Target pitch speed is based in input
		float TargetPitchSpeed = (Val * HoverTurnSpeed * -1.f);

		//// When steering, we decrease pitch slightly
		TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

		//// Smoothly interpolate to target pitch speed
		CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	}
}

void ASpaceShooterPawn::HoverYaw(float Val)
{

	if (bIsHovering)
	{
		// Target yaw speed is based on input
		float TargetYawSpeed = (Val * HoverTurnSpeed);

		// Smoothly interpolate to target yaw speed
		CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

		// Is there any left/right input?
		const bool bIsTurning = FMath::Abs(Val) > 0.2f;
	}
}

void ASpaceShooterPawn::HoverRoll(float Val)
{
	if (bIsHovering)
	{
		// If turning, yaw value is used to influence roll
		// If not turning, rol to reverse current roll value.
		float TargetRollSpeed = (Val * HoverTurnSpeed * -1.f);

		TargetRollSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

		//// Smoothly interpolate roll speed
		CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	}
}

float ASpaceShooterPawn::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Stats->LowerStat(Stats->Health, Damage);
	return 0.0f;
}

