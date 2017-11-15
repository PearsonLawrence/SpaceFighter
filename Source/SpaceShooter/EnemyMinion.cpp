// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyMinion.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"


#include "BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SpaceShooterPawn.h"
#include "ShipStatistics.h"
// Sets default values
AEnemyMinion::AEnemyMinion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = PlaneMesh;

	Cannon1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon1"));		// Only the owning player wil
	Cannon1->SetupAttachment(PlaneMesh, TEXT("CannonCenter"));



	ShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPoint"));
	ShootPoint->SetupAttachment(PlaneMesh, TEXT("CenterShootPoint"));

	Stats = CreateDefaultSubobject<UShipStatistics>(TEXT("ShipStatisticsComponent"));
	Stats->Health = 100;
	Stats->Energy = 100;

	DetectionRadius = 7000;
	ShootDetectionRadius = 3000;
	TurnSpeed = 2; // Uses Time
	ChaseSpeed = 5000; //Uses Clamp
	HoverSpeed = 2; //Uses Time
	ShootDelay = .5f;

	HoverStoppingDistance = 1000;
}

void AEnemyMinion::Fire()
{
	
			FString text = FString::SanitizeFloat(setShootDelay);
			// try and fire a projectile
			if (ProjectileClass != NULL)
			{

				UWorld* const World = GetWorld();
				if (World != NULL)
				{

					const FRotator SpawnRotation = ShootPoint->GetComponentRotation();
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
// Called when the game starts or when spawned
void AEnemyMinion::BeginPlay()
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
void AEnemyMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
	if (ToggleDetection)
	{
		

			if (Player != nullptr)
			{
				if (Player->bIsHovering)
				{
					
					FVector Location;
					Location = (GetActorLocation() - Player->GetActorLocation()).GetSafeNormal() * HoverStoppingDistance + Player->GetActorLocation();


					SetActorLocation(FMath::VInterpTo(GetActorLocation(), Location, DeltaTime, HoverSpeed));

					FRotator Rotation;



					Rotation = FVector(FVector((GetActorLocation() - Player->GetActorLocation()) + Player->GetVelocity()).GetSafeNormal() * -1).Rotation();

					SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaTime, TurnSpeed));
				}
				else
				{
					const FVector LocalMove = FVector(ChaseSpeed * DeltaTime, 0, 0);


					// Move plan forwards (with sweep so we stop when we collide with things)
					AddActorLocalOffset(LocalMove, true);

					//SetActorLocation(FMath::VInterpTo(GetActorLocation(), Player->GetActorLocation(), DeltaTime, MaxSpeed));

					FRotator Rotation;



					Rotation = FVector(FVector((GetActorLocation() - Player->GetActorLocation()) + Player->GetVelocity()).GetSafeNormal() * -1).Rotation();

					SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaTime, TurnSpeed));
				}
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

bool AEnemyMinion::CastSphere(FVector Location, float Radius, bool bShouldDebug)
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

float AEnemyMinion::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Stats->LowerStat(Stats->Health, Damage);
	return 0.0f;
}

