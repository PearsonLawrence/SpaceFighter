// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShooterPawn.generated.h"


UCLASS(Config=Game)
class ASpaceShooterPawn : public APawn
{

	GENERATED_BODY()

		/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Cannon1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Cannon2;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere)
	class UShipStatistics* Stats;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* ShootPointLeft;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* ShootPointRight;

public:
	ASpaceShooterPawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

protected:

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
					
	void FlyMovement(float DeltaSeconds);
	void HoverMovement(float DeltaSeconds);
				
	void ToggleHover();
	// End APawn overrides
	void Fire(float Val);
																							/** Bound to the thrust axis */
	void ThrustInput(float Val);

	/** Bound to the vertical axis */
	void Pitch(float Val);

	/** Bound to the horizontal axis */
	void Yaw(float Val);

	void Roll(float Val);

	void HoverForwardInput(float Val);

	void HoverRightInput(float Val);
	/** Bound to the vertical axis */
	void HoverPitch(float Val);

	/** Bound to the horizontal axis */
	void HoverYaw(float Val);

	void HoverRoll(float Val);
private:

	/** How quickly forward speed changes */
	UPROPERTY(Category = Speed, EditAnywhere)
		float Acceleration;
	UPROPERTY(Category = Speed, EditAnywhere)
		float HoverAcceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Speed, EditAnywhere)
		float TurnSpeed;
	UPROPERTY(Category = Speed, EditAnywhere)
		float HoverTurnSpeed;



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

	UPROPERTY(Category = Speed, EditAnywhere)
		float RotateSpeed;


	UPROPERTY(Category = shooting, EditAnywhere)
		float ShootDelay;
	

	float setShootDelay;
	/** Current forward speed */
	float CurrentForwardSpeed;

	float CurrentUpSpeed;

	float CurrentRightSpeed;
	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	float ForwardVal;
	
	float RightVal;

	float YawVal;

	float ThrustAcc;

	FVector HoverMove;
public:

	//virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UPROPERTY(Category = Flying, EditAnywhere)
	bool bIsHovering;
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
