// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpaceFighterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API USpaceFighterGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	USpaceFighterGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();

	UFUNCTION(Exec)
		void Host();
	UFUNCTION(Exec)
		void Join(const FString& Address);
	
	
	
};
