// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Engine/TargetPoint.h"
#include "Math/BoxSphereBounds.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "TKGameMode.h"
#include "InputCore.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "CoreMinimal.h"

template<typename T>
T* GetCustomGameMode(UWorld* worldContext)
{
	return Cast<T>(worldContext->GetAuthGameMode());
}