// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TKGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MOBTOOKABRI_API ATKGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATKGameMode();

	void CharScoreUp(unsigned int charScore);

	UFUNCTION()
	float GetInvGameSpeed();

	UFUNCTION()
	float GetGameSpeed();

	UFUNCTION()
	int32 GetGameLevel();

	UPROPERTY()
	float gameSpeed;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 numCoinsForSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float gameSpeedIncrease;

	UPROPERTY(BlueprintReadWrite)
	int32 gameLevel;
	
	
};
