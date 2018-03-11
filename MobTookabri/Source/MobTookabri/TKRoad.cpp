// Fill out your copyright notice in the Description page of Project Settings.

#include "TKRoad.h"
#include "MobTookabri.h"


// Sets default values
ATKRoad::ATKRoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>myMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/Street_Mesh_Tookabri.Street_Mesh_Tookabri'"));

	ConstructorHelpers::FObjectFinder<UMaterial>myMaterial(TEXT("Material'/Game/MobileStarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles'"));

	if (myMesh.Succeeded())
	{
		NumRepeatingMesh = 3;

		FBoxSphereBounds myBounds = myMesh.Object->GetBounds();
		float XBounds = myBounds.BoxExtent.X * 2;
		float ScenePos = ((XBounds * (NumRepeatingMesh - 1)) / 2.0f) * -1;

		KillPoint = ScenePos - (XBounds * 0.5f);
		SpawnPoint = (ScenePos * -1) + (XBounds * 0.5f);

		for (int i = 0; i < NumRepeatingMesh; ++i)
		{
			// Initialize Scene
			FString SceneName = "Scene" + FString::FromInt(i);
			FName SceneID = FName(*SceneName);
			USceneComponent* thisScene = CreateDefaultSubobject<USceneComponent>(SceneID);
			check(thisScene);

			thisScene->SetupAttachment(RootComponent);
			thisScene->SetRelativeLocation(FVector(ScenePos, 0.0f, 0.0f));
			ScenePos += XBounds;

			FloorMeshScenes.Add(thisScene);

			// Initialize Mesh
			FString MeshName = "Mesh" + FString::FromInt(i);
			UStaticMeshComponent* thisMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*MeshName));
			check(thisMesh);

			//thisMesh->AttachToComponent(FloorMeshScenes[i], FAttachmentTransformRules::KeepWorldTransform);
			thisMesh->SetupAttachment(FloorMeshScenes[i]);
			thisMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			thisMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

			if (myMaterial.Succeeded())
			{
				thisMesh->SetStaticMesh(myMesh.Object);
				thisMesh->SetMaterial(0, myMaterial.Object);
			}

			FloorMeshes.Add(thisMesh);
		}

		CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBox"));
		check(CollisionBox);

		CollisionBox->SetupAttachment(RootComponent);

		CollisionBox->SetBoxExtent(FVector(SpawnPoint, myBounds.BoxExtent.Y, myBounds.BoxExtent.Z));

		CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}

}

// Called when the game starts or when spawned
void ATKRoad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATKRoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto Scene : FloorMeshScenes)
	{
		Scene->AddLocalOffset(FVector(GetCustomGameMode<ATKGameMode>(GetWorld())->GetInvGameSpeed(), 0.0f, 0.0f));

		if (Scene->GetComponentTransform().GetLocation().X <= KillPoint)
		{
			Scene->SetRelativeLocation(FVector(SpawnPoint, 0.0f, 0.0f));
		}

	}


}

float ATKRoad::GetKillPoint()
{
	return KillPoint;
}

float ATKRoad::GetSpawnPoint()
{
	return SpawnPoint;
}

