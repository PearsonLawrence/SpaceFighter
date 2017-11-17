// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"

 
#include "Engine/EngineTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
ABaseProjectile::ABaseProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);		// set up a notification for when this component hits something blocking

																					// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;


	ProjectileMovement->ProjectileGravityScale = 0;

	CollisionComp->SetSimulatePhysics(false);
	DamageAmount = 10;
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent dmgEvent(ValidDamageTypeClass);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS, GetActorTransform());
/*
	
		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, type);*/
		if (OtherComp->ComponentHasTag(FName("Damage")))
		{
			OtherActor->TakeDamage(DamageAmount, dmgEvent, GetInstigatorController(), this);
		}
		Destroy();
	}
}