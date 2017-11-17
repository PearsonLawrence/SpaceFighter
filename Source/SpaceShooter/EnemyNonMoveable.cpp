// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyNonMoveable.h"


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
AEnemyNonMoveable::AEnemyNonMoveable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("PlaneMesh0"));	// Set static mesh

	PlaneMesh->AttachTo(RootComponent);

	Cannon1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon1"));		// Only the owning player wil
	Cannon1->AttachTo(RootComponent);

	ShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPoint"));
	ShootPoint->AttachTo(Cannon1);

	Stats = CreateDefaultSubobject<UShipStatistics>(TEXT("ShipStatisticsComponent"));
	Stats->Health = 100;
	Stats->Energy = 100;

	DetectionRadius = 7000;
	ShootDetectionRadius = 3000;
	TurnSpeed = 2; // Uses Time
	ShootDelay = .5f;

	HoverStoppingDistance = 1000;
}

void AEnemyNonMoveable::Fire()
{

	FString text = FString::SanitizeFloat(setShootDelay);
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{

		UWorld* const World = GetWorld();
		if (World != NULL)
		{

			const FRotator SpawnRotation = FVector(FVector((ShootPoint->GetComponentLocation() - Player->GetActorLocation()).GetSafeNormal() * -1)).Rotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((ShootPoint != nullptr) ? ShootPoint->GetComponentLocation() : ShootPoint->GetComponentLocation());

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			World->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}



}
void AEnemyNonMoveable::LookMovement(float DeltaTime)
{
	FRotator Rotation = FVector(FVector(GetActorLocation() - Player->GetActorLocation()).GetSafeNormal() * -1).Rotation();

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaTime, TurnSpeed));
}
// Called when the game starts or when spawned
void AEnemyNonMoveable::BeginPlay()
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
void AEnemyNonMoveable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (ToggleDetection && bIsDead == false)
	{


		if (Player != nullptr)
		{
			LookMovement(DeltaTime);

			setShootDelay -= DeltaTime;

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

bool AEnemyNonMoveable::CastSphere(FVector Location, float Radius, bool bShouldDebug)
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

float AEnemyNonMoveable::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Stats->LowerStat(Stats->Health, Damage);
	if (Stats->Health <= 0)
	{
		PlaneMesh->ApplyDamage(ExplodeDamage, GetActorLocation(), GetActorLocation(), ExplodeStrength);
		PlaneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlaneMesh->SetSimulatePhysics(true);
		Cannon1->SetVisibility(false);
		Cannon1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bIsDead = true;
		Player->Score++;
	}
	return 0.0f;
}
