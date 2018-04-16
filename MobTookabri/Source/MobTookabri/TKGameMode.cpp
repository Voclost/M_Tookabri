// Fill out your copyright notice in the Description page of Project Settings.

#include "TKGameMode.h"

ATKGameMode::ATKGameMode()
{
	// set default pawn class to our ABountyDashCharacter
	DefaultPawnClass = ATKGameMode::StaticClass();

	numCoinsForSpeedIncrease = 75;
	gameSpeed = 10.0f;
	gameSpeedIncrease = 0.03f;
	gameLevel = 1;
}

void ATKGameMode::CharScoreUp(unsigned int charScore)
{
	if (charScore != 0 && charScore % numCoinsForSpeedIncrease == 0)
	{
		gameSpeed += gameSpeedIncrease;
		gameLevel++;
	}
}

float ATKGameMode::GetInvGameSpeed()
{
	return -gameSpeed;
}

float ATKGameMode::GetGameSpeed()
{
	return gameSpeed;
}

int32 ATKGameMode::GetGameLevel()
{
	return gameLevel;
}

float ATKGameMode::SetGameSpeed(float NewGameSpeed)
{
	gameSpeed = NewGameSpeed;
	return gameSpeed;
}

float ATKGameMode::GetRunTime()
{
	return RunTime;
}

void ATKGameMode::Tick(float DeltaTime)
{
	RunTime += DeltaTime;
}
