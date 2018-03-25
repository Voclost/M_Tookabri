// Fill out your copyright notice in the Description page of Project Settings.

#include "TKCharacter.h"
#include "Obstacle.h"
#include "MobTookabri.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprint.h"
#include "EngineUtils.h"


// Sets default values
ATKCharacter::ATKCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>myMesh(TEXT("SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'"));

	// Set size for collision capsule
	GetCapsuleComponent()->SetCapsuleSize(80.0f, 40.0f);

	if (myMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myMesh.Object);
	}

	// Rotate and position the mesh so it sits in the capsule component properly
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Configure character movement
	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check(CameraBoom);

	//CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	CameraBoom->SetupAttachment(RootComponent);

	// The camera follows at this distance behind the character
	CameraBoom->TargetArmLength = 220.0f;

	// Offset to player
	CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.0f));
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	check(FollowCamera);

	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Rotational change to make the camera look down slightly
	FollowCamera->RelativeRotation = FRotator(-10.0f, 0.0f, 0.0f);

	// Game Properties
	CharSpeed = 10.0f;

	// Poses the input at ID 0 (the default controller)
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	moveSpeed = 8.0f;

}

// Called when the game starts or when spawned
void ATKCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATKCharacter::myOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ATKCharacter::myOnComponentEndOverlap);

	for (TActorIterator<ATargetPoint>TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		if (TargetIter->ActorHasTag(TEXT("TargetPlayer")))
		{
			TargetArray.Add(*TargetIter);
		}
		
	}

	auto SortPred = [](const AActor& A, const AActor& B)->bool
	{
		return(A.GetActorLocation().Y < B.GetActorLocation().Y);
	};
	TargetArray.Sort(SortPred);

	CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);

	// Character initial scale
	this->SetActorScale3D(FVector(0.6f,0.6f,0.6f));
}

void ATKCharacter::ScoreUp()
{
}



void ATKCharacter::MoveRight()
{
	if ((Controller != nullptr))
	{
		if (CurrentLocation < TargetArray.Num() - 1)
		{
			++CurrentLocation;
		}
		else
		{
			// Do Nothing
		}
	}
}

void ATKCharacter::MoveLeft()
{
	if ((Controller != nullptr))
	{
		if (CurrentLocation > 0)
		{
			--CurrentLocation;
		}
		else
		{
			// Do Nothing
		}
	}
}


void ATKCharacter::myOnComponentOverlap(UPrimitiveComponent* ThisActor, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SwwepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OUFFEOUW MEN DA5LOU DANS LE IF"));
	/*if (otherActor->GetClass()->IsChildOf(AObstacle::StaticClass()))
	{
		FVector vecBetween = otherActor->GetActorLocation() - GetActorLocation();
		float AngleBetween = FMath::Acos(FVector::DotProduct(vecBetween.GetSafeNormal(), GetActorForwardVector().GetSafeNormal()));
		
		AngleBetween *= (180 / PI);
		
		if (AngleBetween < 60.0f)
		{
			bBeingPushed = true;
		}
	}*/

}

void ATKCharacter::myOnComponentEndOverlap(UPrimitiveComponent* ThisActor, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	/*if (otherActor->GetClass()->IsChildOf(AObstacle::StaticClass()))
	{
		bBeingPushed = false;
	}*/
}


// Called every frame
void ATKCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TargetArray.Num() > 0)
	{
		FVector targetLoc = TargetArray[CurrentLocation]->GetActorLocation();
		targetLoc.Z = GetActorLocation().Z;
		targetLoc.X = GetActorLocation().X;
		if (targetLoc != GetActorLocation())
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), targetLoc, CharSpeed* DeltaTime));
		}
	}

	//ATKGameMode* SetGameSpeed(float moveSpeed);
	//GetCustomGameMode<ATKGameMode>(GetWorld()->SetGameSpeed(float moveSpeed));
	// UE_LOG(YourLog,Warning,TEXT("MyCharacter's Health is %f"), MyCharacter->Health );
	//UE_LOG(LogClass, Log, TEXT("MoveSpeed Value: %f"), this->moveSpeed);
	if (moveSpeed == 0.0f)
	{
		float gameSpeed = GetCustomGameMode<ATKGameMode>(GetWorld())->SetGameSpeed(moveSpeed);
	}
	
}




// Called to bind functionality to input
void ATKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ATKCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ATKCharacter::MoveLeft);
		

}

