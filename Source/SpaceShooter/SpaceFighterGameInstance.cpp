// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceFighterGameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"



USpaceFighterGameInstance::USpaceFighterGameInstance(const FObjectInitializer& ObjectInitializer)
{
	


}

void USpaceFighterGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));

}


