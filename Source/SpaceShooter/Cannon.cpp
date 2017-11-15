// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"


// Sets default values for this component's properties
UCannonComponent::UCannonComponent()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> CannonMesh;
		FConstructorStatics()
			: CannonMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetStaticMesh(ConstructorStatics.CannonMesh.Get());

	// ...
}


void UCannonComponent::Fire()
{
}

void UCannonComponent::CoolDown()
{
}

