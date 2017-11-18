// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();
private:

	UPROPERTY(meta = (BindWidget))
	class UButton* Start;

	UPROPERTY(meta = (BindWidget))
	class UButton* Quit;
	UPROPERTY(EditAnywhere, Category = level)
		FName Level;

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void QuitGame();

	
};
