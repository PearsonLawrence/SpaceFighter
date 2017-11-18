#include "ShipStatistics.h"
// Fill out your copyright notice in the Description page of Project Settings.
#include "Engine/World.h"

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

void UShipStatistics::RaiseStat(float & Stat, float RaiseAmount)
{
	Stat += RaiseAmount;
}

void UShipStatistics::Die(AActor* ActorToDie, bool bIsPlayer)
{
	if (ActorToDie != nullptr)
	{
		if (!bIsPlayer)
			GetWorld()->DestroyActor(ActorToDie);
		else
			GetWorld()->GetFirstPlayerController()->UnPossess();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Null Reference for Actor To Die"));
	}
}

