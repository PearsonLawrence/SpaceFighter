// Fill out your copyright notice in the Description page of Project Settings.

#include "LargeEnemy.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"


#include "BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SpaceShooterPawn.h"
#include "ShipStatistics.h"
#include "DestructibleComponent.h"
// Sets default values
ALargeEnemy::ALargeEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	
	ShipParts = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Base"));		// Only the owning player wil
	ShipParts->AttachTo(RootComponent);
	
	ShipParts2 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Part1"));		// Only the owning player wil
	ShipParts2->AttachTo(ShipParts);
	
	ShipParts3 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Part2"));		// Only the owning player wil
	ShipParts3->AttachTo(ShipParts);
	
	ShipParts4 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Part3"));		// Only the owning player wil
	ShipParts4->AttachTo(ShipParts);

	ShipParts5 = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Part4"));		// Only the owning player wil
	ShipParts5->AttachTo(ShipParts);

	RootComponent = ShipParts;

	Cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon1"));		// Only the owning player wil
	Cannon->AttachTo(RootComponent);

	Cannon2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon2"));		// Only the owning player wil
	Cannon2->AttachTo(RootComponent);

	Cannon3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon3"));		// Only the owning player wil
	Cannon3->AttachTo(RootComponent);

	Cannon4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon4"));		// Only the owning player wil
	Cannon4->AttachTo(RootComponent);

	Cannon5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon5"));		// Only the owning player wil
	Cannon5->AttachTo(RootComponent);

	Cannon6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon6"));		// Only the owning player wil
	Cannon6->AttachTo(RootComponent);

	
	ShootPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootPoint"));		// Only the owning player wil
	ShootPoint->AttachTo(Cannon);

	ShootPoint2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootPoint2"));		// Only the owning player wil
	ShootPoint2->AttachTo(Cannon2);

	ShootPoint3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootPoint3"));		// Only the owning player wil
	ShootPoint3->AttachTo(Cannon3);

	ShootPoint4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootPoint4"));		// Only the owning player wil
	ShootPoint4->AttachTo(Cannon4);

	ShootPoint5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootPoint5"));		// Only the owning player wil
	ShootPoint5->AttachTo(Cannon5);

	ShootPoint6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootPoint6"));		// Only the owning player wil
	ShootPoint6->AttachTo(Cannon6);

	Parts.Add(ShipParts);
	Parts.Add(ShipParts2);
	Parts.Add(ShipParts3);
	Parts.Add(ShipParts4);
	Parts.Add(ShipParts5);


	Cannons.Add(Cannon);
	Cannons.Add(Cannon2);
	Cannons.Add(Cannon3);
	Cannons.Add(Cannon4);
	Cannons.Add(Cannon5);
	Cannons.Add(Cannon6);

	ShootPoints.Add(ShootPoint);
	ShootPoints.Add(ShootPoint2);
	ShootPoints.Add(ShootPoint3);
	ShootPoints.Add(ShootPoint4);
	ShootPoints.Add(ShootPoint5);
	ShootPoints.Add(ShootPoint6);


	Stats = CreateDefaultSubobject<UShipStatistics>(TEXT("ShipStatisticsComponent"));
	Stats->Health = 100;
	Stats->Energy = 100;

	DetectionRadius = 7000;
	ShootDetectionRadius = 3000;
	TurnSpeed = 2; // Uses Time
	ShootDelay = .5f;

	HoverStoppingDistance = 1000;
}

void ALargeEnemy::Fire()
{

	FString text = FString::SanitizeFloat(setShootDelay);
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			for (int i = 0; i < 4; i++)
			{
				const FRotator SpawnRotation = (i < 2) ? FVector(FVector(ShootPoints[i]->GetComponentLocation() - Player->GetActorLocation()).GetSafeNormal() * -1).Rotation() : ShootPoints[i]->GetComponentRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((ShootPoints[i] != nullptr) ? ShootPoints[i]->GetComponentLocation() : ShootPoints[i]->GetComponentLocation());

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// spawn the projectile at the muzzle
				World->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}




}
void ALargeEnemy::SpinMovement(float DeltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		FQuat tmp = FQuat({ 0, 0, 1 }, DeltaTime * 2);

	
		Cannons[i]->SetRelativeRotation(Cannons[i]->GetRelativeTransform().GetRotation() * tmp);
	
	}
	for (int i = 0; i < 3; i++)
	{
		
		if (i != 0)
		{
			FQuat tmp2 = FQuat({ 0, 0, -1 }, DeltaTime * 2);

			
			Parts[i]->SetRelativeRotation(Parts[i]->GetRelativeTransform().GetRotation() * tmp2);
		}
		else
		{

			FQuat tmp2 = FQuat({ 0, 0, 1 }, DeltaTime * -2);

			Parts[i]->SetRelativeRotation(Parts[i]->GetRelativeTransform().GetRotation() * tmp2);
		}
	}

}
void ALargeEnemy::LookMovement(float DeltaTime)
{
	FRotator Rotation = FVector(FVector(GetActorLocation() - Player->GetActorLocation()).GetSafeNormal() * -1).Rotation();

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaTime, TurnSpeed));
}
// Called when the game starts or when spawned
void ALargeEnemy::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpaceShooterPawn::StaticClass(), FoundActors);

	if (FoundActors[0] != nullptr)
	{
		Player = Cast<ASpaceShooterPawn>(FoundActors[0]);

	}
}

// Called every frame
void ALargeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (ToggleDetection && bIsDead == false)
	{
		
		if (Player != nullptr)
		{
			SpinMovement(DeltaTime);

			LookMovement(DeltaTime);

			setShootDelay -= DeltaTime;

			if (CastSphere(GetActorLocation(), RetreatRadius))
			{
				FVector Location;
				Location = (GetActorLocation() - Player->GetActorLocation()).GetSafeNormal() * HoverStoppingDistance + Player->GetActorLocation();


				SetActorLocation(FMath::VInterpTo(GetActorLocation(), Location, DeltaTime, Acceleration));

			
			}

			if (CastSphere(GetActorLocation(), ShootDetectionRadius))
			{
				if (setShootDelay <= 0)
				{
					setShootDelay = ShootDelay;
					Fire();
				}
			}

		}

	}
	else
	{
		if (CastSphere(GetActorLocation(), DetectionRadius))
		{
			ToggleDetection = true;
		}
	}

}

bool ALargeEnemy::CastSphere(FVector Location, float Radius, bool bShouldDebug)
{
	FHitResult HitResult;
	bool hashit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(Radius)
	);
	if (bShouldDebug)
	{
		FColor ResultColor = hashit ? FColor::Green : FColor::Red;

		DrawDebugSphere(GetWorld(), Location, Radius, 45, ResultColor, false, 0);

	}
	return hashit;
}

float ALargeEnemy::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Stats->LowerStat(Stats->Health, Damage);
	if (Stats->Health <= 0)
	{
		for (int i = 0; i < 5; i++)
		{
			
				Parts[i]->ApplyDamage(ExplodeDamage, GetActorLocation(), GetActorLocation(), ExplodeStrength);
				Parts[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			

			if (Cannons[i] != nullptr)
			{
				Cannons[i]->SetVisibility(false);
				Cannons[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		
		bIsDead = true;
		Player->Score++;
	}
	return 0.0f;
}
