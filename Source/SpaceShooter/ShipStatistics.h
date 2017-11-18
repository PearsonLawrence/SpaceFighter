// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShipStatistics.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACESHOOTER_API UShipStatistics : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipStatistics();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(Category = PlayerStats, EditAnywhere, BlueprintReadOnly)
	float Health;

	UPROPERTY(Category = PlayerStats, EditAnywhere, BlueprintReadOnly)
	float Energy;

	UPROPERTY(Category = PlayerStats, EditAnywhere, BlueprintReadOnly)
	int32 ShipKills;


	void LowerStat(float &Stat, float LowerAmount);

	void RaiseStat(float &Stat, float RaiseAmount);

	void Die(AActor* ActorToDie, bool bIsPlayer = false);

};
