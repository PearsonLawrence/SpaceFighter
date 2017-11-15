
// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipStatistics.h"


// Sets default values for this component's properties
UShipStatistics::UShipStatistics()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShipStatistics::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShipStatistics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShipStatistics::LowerStat(float & Stat, float LowerAmount)
{
	Stat -= LowerAmount;
}

void UShipStatistics::Die(bool bIsPlayer)
{
	
}

