// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LargeEnemy.generated.h"

UCLASS()
class SPACESHOOTER_API ALargeEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALargeEnemy();


	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* ShipParts;
	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* ShipParts2;
	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* ShipParts3;
	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* ShipParts4;
	UPROPERTY(Category = Mesh, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UDestructibleComponent* ShipParts5;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint2;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint3;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint4;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint5;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint6;


	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon2;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon3;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon4;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon5;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon6;

	TArray<class UStaticMeshComponent*> Cannons;
	TArray<class UDestructibleComponent*> Parts;
	TArray<class USceneComponent*> ShootPoints;

	class UDestructibleComponent* tempParts;
	class USceneComponent* tempShootPoints;
	class UStaticMeshComponent* tempCannon;


	UPROPERTY(VisibleAnywhere)
		class UShipStatistics* Stats;

	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class ABaseProjectile> ProjectileClass;


	UPROPERTY(VisibleDefaultsOnly, Category = Reference)
		class ASpaceShooterPawn* Player;

	bool ToggleDetection;
	bool bIsDead;

	void Fire();


	void SpinMovement(float DeltaTime);
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
	UPROPERTY(Category = Detection, EditAnywhere)
		float RetreatRadius;

	UPROPERTY(Category = Chase, EditAnywhere)
		float HoverStoppingDistance;


	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


	bool CastSphere(FVector Location, float Radius, bool bShouldDebug = false);


	
};
