// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SpaceShooterGameMode.h"
#include "SpaceShooterPawn.h"

#include "UObject/ConstructorHelpers.h"

ASpaceShooterGameMode::ASpaceShooterGameMode()
{
	// set default pawn class to our flying pawn
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Flying/Blueprints/Spaceship_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
