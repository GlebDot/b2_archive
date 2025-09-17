// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/SpawnComponents/BurstSpawnComponent.h"
#include "BlundersGame/Public/ArenaMaster/ArenaMaster.h"
#include "BlundersGame/Public/ArenaMaster/EnemySpawnPoint.h"

// Sets default values for this component's properties
UBurstSpawnComponent::UBurstSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UBurstSpawnComponent::BeginSpawn_Implementation(AArenaMaster *ArenaMasterRef)
{
	ArenaRef = ArenaMasterRef;
	//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "Hello from spawner");

	switch (SpawnType)
	{
		case EBurstSpawnType::SingleBurst:
			SpawnBurstCounter = 1;
			break;
		case EBurstSpawnType::DoubleBurst:
			SpawnBurstCounter = 2;
			break;
		case EBurstSpawnType::TripleBurst:
			SpawnBurstCounter = 3;
			break;
		default:
			break;
	}

	SpawnEnemyTypesAmount = FMath::CeilToInt(EnemyArray.Num() * 1.0 / SpawnBurstCounter);

	GetWorld()->GetTimerManager().SetTimer(SpawnDelayTimerHandle, this, &UBurstSpawnComponent::SpawnBurst, SpawnBurstsDelayTime, false);
}

void UBurstSpawnComponent::SpawnBurst()
{
	//Clearing timers
	GetWorld()->GetTimerManager().ClearTimer(SpawnDelayTimerHandle);
	int CurrentSpawnPointIndex = 0;
	float DelayTime = 0.0;
	TArray<AEnemySpawnPoint*> SpawnPoints = ArenaRef->GetSpawnPoints();
	
	for (int i = 0; i < EnemyArray.Num(); i++)
	{
		if (i % SpawnEnemyTypesAmount == 0 && i != 0)
		{
			DelayTime += SpawnBurstsDelayTime * (i / SpawnEnemyTypesAmount);
		}
		
		for (int j = 0; j < EnemyArray[i].EnemyCount; j++)
		{
			int SpawnPointIndex = CurrentSpawnPointIndex % SpawnPoints.Num();
			DelayTime += SpawnEnemiesDelayTime;
			SpawnPoints[SpawnPointIndex]->AddEnemyEntryToSpawn(EnemyArray[i].EnemyType, DelayTime);
			CurrentSpawnPointIndex++;
		}
	}

	//Notify Arena Master about the end of spawn
	FTimerHandle BurstEndTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(BurstEndTimerHandle, this, &UBurstSpawnComponent::SpawnEnd, DelayTime, false);
}

