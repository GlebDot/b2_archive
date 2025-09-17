// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerMovementObjects/JumperActor.h"
#include "GameFramework/Character.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerMovementStates/PlayerJumpStates.h"

// Sets default values
AJumperActor::AJumperActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	JumperCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	if (JumperCollision)
	{
		SetRootComponent(JumperCollision);
		JumperCollision->SetCollisionProfileName(FName("Trigger"), true);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(FName("NoCollision"), true);
	Mesh->SetupAttachment(JumperCollision);

}

// Called when the game starts or when spawned
void AJumperActor::BeginPlay()
{
	Super::BeginPlay();

	if (JumperCollision)
	{
		JumperCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &AJumperActor::OnOverlapBegin);
	}
}

void AJumperActor::LaunchPlayer(APlayerCharacter *Character, const FVector& LaunchVelocity)
{
	Character->ResetPlayerJumps();
	Character->SetCurrentState(EPlayerMovementStates::JumpJumper, 0, LaunchVelocity);

	CanLaunch = false;
	GetWorld()->GetTimerManager().SetTimer(JumperResetTimerHandle, this, &AJumperActor::ResetJumper, CooldownTime, false);
}

void AJumperActor::ResetJumper()
{
	CanLaunch = true;
	GetWorld()->GetTimerManager().ClearTimer(JumperResetTimerHandle);
}

void AJumperActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanLaunch)
	{
		APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			LaunchPlayer(Player, FVector(0, 0, JumpVelocity));
		}
	}
}

