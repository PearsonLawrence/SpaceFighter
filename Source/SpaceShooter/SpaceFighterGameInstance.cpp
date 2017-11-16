// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceFighterGameInstance.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"



USpaceFighterGameInstance::USpaceFighterGameInstance(const FObjectInitializer& ObjectInitializer)
{
	/*ConstructorHelpers::FClassFinder<APlatformTrigger> PlatformTriggerBPClass(TEXT("/Game/PuzzlePlatforms/PlatformTrigger_BP"));
*/
	/*if (!ensure(PlatformTriggerBPClass.Class != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *PlatformTriggerBPClass.Class->GetName());
*/
}

void USpaceFighterGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));

}

void USpaceFighterGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	world->ServerTravel("/Game/FlyingCPP/Maps/FlyingExampleMap?listen");
}

void USpaceFighterGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* Player = GetFirstLocalPlayerController();

	if (!ensure(Player != nullptr)) return;

	Player->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
