// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/EnemySpawnPoint.h"
#include "Components/BillboardComponent.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"

// Sets default values
AEnemySpawnPoint::AEnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}

#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite Component"));
	SpriteComponent->SetupAttachment(SceneComponent);
#endif
}

void AEnemySpawnPoint::Reset()
{
	Super::Reset();
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	EnemySpawnQueue.Empty();
}

void AEnemySpawnPoint::SetArenaMaster(AArenaMaster* Arena)
{
	ArenaMasterRef = Arena;
}

AArenaMaster* AEnemySpawnPoint::GetArenaMaster() const
{
	return ArenaMasterRef;
}

// Called when the game starts or when spawned
void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawnPoint::SpawnEnemy()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	
	const FVector SpawnLocation = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEnemyParent* Tmp = GetWorld()->SpawnActor<AEnemyParent>(EnemySpawnQueue[0].EnemyType, SpawnLocation, FRotator(0,0,0), SpawnParams);

	ArenaMasterRef->RegisterEnemy(Tmp);

	EnemySpawnQueue.RemoveAtSwap(0);
	
	if (EnemySpawnQueue.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnPoint::SpawnEnemy, EnemySpawnQueue[0].DelayTime, false);
	}
}

void AEnemySpawnPoint::AddEnemyEntryToSpawn(const TSubclassOf<AEnemyParent>& EnemyType, float GlobalDelayTime)
{
	if (EnemySpawnQueue.Num() == 0)
	{
		EnemySpawnQueue.Add(FEnemySpawnEntry(EnemyType, GlobalDelayTime));
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawnPoint::SpawnEnemy, EnemySpawnQueue[0].DelayTime, false);
	} else
	{
		float CurrentDelay = EnemySpawnQueue.Top().DelayTime;
		CurrentDelay = GlobalDelayTime - CurrentDelay;

		EnemySpawnQueue.Add(FEnemySpawnEntry(EnemyType, CurrentDelay));
	}
}

