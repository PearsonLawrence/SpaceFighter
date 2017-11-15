// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyMinion.generated.h"

UCLASS()
class SPACESHOOTER_API AEnemyMinion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyMinion();


	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PlaneMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* ShootPoint;

	
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Cannon1;

	UPROPERTY(VisibleDefaultsOnly, Category = Reference)
		class ASpaceShooterPawn* Player;

	bool ToggleDetection;


	void Fire();

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
	UPROPERTY(Category = Speed, EditAnywhere)
		float HoverAcceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Speed, EditAnywhere)
		float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Speed, EditAnywhere)
		float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Speed, EditAnywhere)
		float MinSpeed;

	UPROPERTY(Category = Speed, EditAnywhere)
		float HoverMaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Speed, EditAnywhere)
		float HoverMinSpeed;
	UPROPERTY(Category = Speed, EditAnywhere)
		float HoverDrag;
	UPROPERTY(Category = Detection, EditAnywhere)
		float DetectionRadius;

	bool CastSphere(FVector Location, float Radius);
};
