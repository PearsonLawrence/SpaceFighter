// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyNonMoveable.generated.h"

UCLASS()
class SPACESHOOTER_API AEnemyNonMoveable : public AActor
{
	GENERATED_BODY()
	
public:		
	AEnemyNonMoveable();



	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* PlaneMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint;


	UPROPERTY(VisibleAnywhere)
		class UShipStatistics* Stats;

	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon1;

	UPROPERTY(VisibleDefaultsOnly, Category = Reference)
		class ASpaceShooterPawn* Player;

	bool ToggleDetection;
	bool bIsDead;

	void Fire();

	void LookMovement(float DeltaTime);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(Category = shooting, EditAnywhere)
		float ShootDelay;

	float setShootDelay;
	/** How quickly forward speed changes */
	UPROPERTY(Category = Speed, EditAnywhere)
		float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Speed, EditAnywhere)
		float TurnSpeed;

	UPROPERTY(Category = Damage, EditAnywhere)
		float ExplodeStrength;
	UPROPERTY(Category = Damage, EditAnywhere)
		float ExplodeDamage;

	
	/** Min forward speed */

	UPROPERTY(Category = Detection, EditAnywhere)
		float DetectionRadius;
	UPROPERTY(Category = Detection, EditAnywhere)
		float ShootDetectionRadius;

	UPROPERTY(Category = Chase, EditAnywhere)
		float HoverStoppingDistance;


	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	
	bool CastSphere(FVector Location, float Radius, bool bShouldDebug = false);
	
	
};
