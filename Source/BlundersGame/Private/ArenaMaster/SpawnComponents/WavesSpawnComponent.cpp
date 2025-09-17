// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/SpawnComponents/WavesSpawnComponent.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "BlundersGame/Public/ArenaMaster/EnemySpawnPoint.h"

// Sets default values for this component's properties
UWavesSpawnComponent::UWavesSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UWavesSpawnComponent::BeginSpawn_Implementation(AArenaMaster* ArenaMasterRef)
{
	ArenaRef = ArenaMasterRef;

	ArenaRef->WeightReceivedDelegate.Unbind();
	ArenaRef->WeightReducedDelegate.Unbind();

	ArenaRef->WeightReceivedDelegate.BindUFunction(this, FName("OnWeightAdded"));
	ArenaRef->WeightReducedDelegate.BindUFunction(this, FName("OnWeightReduced"));
	
	CurrentWaveSpawnIndex = 0;

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UWavesSpawnComponent::SpawnEnemyWave, SpawnBeginDelayTime, false);
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, "Wave Spawn");
}

// Called when the game starts
void UWavesSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWavesSpawnComponent::OnWeightAdded(const float Weight)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, "WEIGTH ADDED");
	CurrentWaveWeight += Weight;
}

void UWavesSpawnComponent::OnWeightReduced()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, "WEIGTH REDUCED");
	if (CanSpawnNewWave)
	{
		const float CurrentWeightPercent = ArenaRef->GetArenaWeight() / CurrentWaveWeight;
		GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Blue, FString::SanitizeFloat(CurrentWeightPercent));
		if (CurrentWeightPercent < WaveSettings[CurrentWaveSpawnIndex].WeightThreshold)
		{
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &UWavesSpawnComponent::SpawnEnemyWave, SpawnBeginDelayTime, false);
			CanSpawnNewWave = false;
		}
	}
}

void UWavesSpawnComponent::SpawnEnemyWave()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	CurrentWaveWeight = 0.0;
	
	float SpawnDelayGlobal = 0.0;
	int CurrentSpawnPointIndex = 0;
	
	TArray<AEnemySpawnPoint*> SpawnPoints = ArenaRef->GetSpawnPoints();

	if (IsDefaultWave && DefaultWaveSettings.Num() > 0)
	{
		for (int i = 0; i < DefaultWaveSettings.Num(); i++)
		{
			for (int j = 0; j < DefaultWaveSettings[i].EnemyCount; j++)
			{
				SpawnDelayGlobal += SpawnEnemyDelayTime;
				int SpawnPointIndex = CurrentSpawnPointIndex % SpawnPoints.Num();
				SpawnPoints[SpawnPointIndex]->AddEnemyEntryToSpawn(DefaultWaveSettings[i].EnemyType, SpawnDelayGlobal);
				CurrentSpawnPointIndex++;
			}
		}
	}

	for (int i = 0; i < WaveSettings[CurrentWaveSpawnIndex].WaveEnemiesSet.Num(); i++)
	{
		for(int j = 0; j < WaveSettings[CurrentWaveSpawnIndex].WaveEnemiesSet[i].EnemyCount; j++)
		{
			SpawnDelayGlobal += SpawnEnemyDelayTime;
			int SpawnPointIndex = CurrentSpawnPointIndex % SpawnPoints.Num();
			SpawnPoints[SpawnPointIndex]->AddEnemyEntryToSpawn(WaveSettings[CurrentWaveSpawnIndex].WaveEnemiesSet[i].EnemyType, SpawnDelayGlobal);
			CurrentSpawnPointIndex++;
		}
	}

	CurrentWaveSpawnIndex++;
	if (CurrentWaveSpawnIndex == WaveSettings.Num())
	{
		ArenaRef->WeightReceivedDelegate.Unbind();
		ArenaRef->WeightReducedDelegate.Unbind();
		
		//Notify Arena Master about spawn Ending;
		SpawnEnd();
	} else
	{
		GetWorld()->GetTimerManager().SetTimer(NewWaveTimerHandle, this, &UWavesSpawnComponent::AllowSpawnNewWave, SpawnDelayGlobal, false);
	}
}

void UWavesSpawnComponent::AllowSpawnNewWave()
{
	CanSpawnNewWave = true;
}


// Called every frame
void UWavesSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

