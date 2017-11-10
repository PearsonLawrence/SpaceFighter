// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShooterPawn.generated.h"

USTRUCT()
struct FInputStruct
{
	GENERATED_BODY()

	UPROPERTY()
	float PitchInput;
	UPROPERTY()
	float YawInput;
	UPROPERTY()
	float RollInput;
	UPROPERTY()
	float ThrustInput;
	UPROPERTY()
	float BreakInput;
};

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
public:
	ASpaceShooterPawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides
protected:

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the thrust axis */
	void ThrustInput(float Val);

	
	/** Bound to the vertical axis */
	
	void UpdatePitch(float Val);

	void UpdateYaw(float Val);

	void UpdateRoll(float Val);

	void UpdateThrust(float Val);



	UFUNCTION(NetMulticast, reliable, WithValidation)
	void UpdateLocationFromServer(FInputStruct Input, float DeltaSeconds);
	
	/** Bound to the horizontal axis */
	void Yaw(float Val);

	void Roll(float Val);

private:


	FInputStruct ShipInput;

	float ticktime;
	float SetTickTime = .09;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
	/** How quickly forward speed changes */
	UPROPERTY(Category=Plane, EditAnywhere)
	float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category=Plane, EditAnywhere)
	float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category=Yaw, EditAnywhere)
	float MinSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

public:

	UPROPERTY(Replicated)
	float Health;
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
