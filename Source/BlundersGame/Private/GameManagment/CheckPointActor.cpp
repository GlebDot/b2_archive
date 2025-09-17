// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/GameManagment/CheckPointActor.h"

#include "GameManagment/BlundersGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase/PlayerCharacter.h"

// Sets default values
ACheckPointActor::ACheckPointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName("Trigger", true);
	BoxCollision->SetupAttachment(SceneComponent);
}

void ACheckPointActor::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACheckPointActor::OnCheckPointBeginOverlap);
}

void ACheckPointActor::OnCheckPointBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, "CHECK");
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACheckPointActor::OnCheckPointBeginOverlap);
		ABlundersGameModeBase *GameMode = Cast<ABlundersGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, "POINT");
			GameMode->SetCurrentCheckPoint(this);
		}
	}
}

