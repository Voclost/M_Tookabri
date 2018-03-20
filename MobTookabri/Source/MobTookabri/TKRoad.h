// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TKRoad.generated.h"

UCLASS()
class MOBTOOKABRI_API ATKRoad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATKRoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetKillPoint();
	float GetSpawnPoint();
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Road")
	TArray<USceneComponent*> FloorMeshScenes;

	UPROPERTY(EditAnywhere, Instanced, Category = "Road")
	TArray<UStaticMeshComponent*> FloorMeshes;

	UPROPERTY(EditAnywhere, Instanced, Category = "Road")
	UBoxComponent* CollisionBox;

	int32 NumRepeatingMesh;
	float KillPoint;
	float SpawnPoint;
};
