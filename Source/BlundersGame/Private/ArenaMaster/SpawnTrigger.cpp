// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/SpawnTrigger.h"

#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "BlundersGame/Public/ArenaMaster/ArenaMaster.h"

// Sets default values
ASpawnTrigger::ASpawnTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trigger"));
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxTrigger->SetCollisionProfileName(FName("Trigger"), true);
	if (BoxTrigger)
	{
		SetRootComponent(BoxTrigger);
	}

#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite Component"));

	if (SpriteComponent)
	{
		SpriteComponent->SetupAttachment(BoxTrigger);
	}
#endif
}

// Called when the game starts or when spawned
void ASpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASpawnTrigger::OnOverlap);
}

void ASpawnTrigger::Reset()
{
	Super::Reset();
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASpawnTrigger::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Emerald, "Spawn Begin");
	APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnBegin();
	}
}

void ASpawnTrigger::SpawnBegin()
{
	ArenaMasterRef->SpawnStart();
}

