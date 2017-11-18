// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	//TODO: setup
	Start->OnClicked.AddDynamic(this, &UMainMenu::StartGame);
	Quit->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	return true;
}

void UMainMenu::StartGame()
{
	
		UGameplayStatics::OpenLevel(GetWorld(), Level);
	

}
void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit);
}
