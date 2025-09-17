// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/ArenaMaster/ArenaMaster.h"
#include "BlundersGame/Public/EnemyAI/EnemyTargetPoint.h"
#include "BlundersGame/Public/ArenaMaster/EnemySpawnPoint.h"
#include "ArenaMaster/SpawnComponents/Spawner.h"
#include "Components/BillboardComponent.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"

void AArenaMaster::AddTickets(int Tickets)
{
	ArenaTickets += Tickets;
}

void AArenaMaster::DecreaseTickets(int Tickets)
{
	ArenaTickets -= Tickets;
}

int AArenaMaster::GetTickets() const
{
	return ArenaTickets;
}

// Sets default values
AArenaMaster::AArenaMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite Component"));
	if (SpriteComponent)
	{
		SetRootComponent(SpriteComponent);
	}
#endif
}

void AArenaMaster::UnRegisterEnemy(AEnemyParent* Enemy)
{
	SpawnedEnemies.Remove(Enemy);
}

void AArenaMaster::RegisterEnemy(AEnemyParent* Enemy)
{
	SpawnedEnemies.Add(Enemy);
	if (Enemy)
	{
		Enemy->RegisterArena(this);
	}
}

void AArenaMaster::AddWeight(float Weight)
{
	ArenaWeightSum += Weight;
	if (WeightReceivedDelegate.IsBound())
	{
		WeightReceivedDelegate.Execute(Weight);
	}
}

void AArenaMaster::DecreaseWeight(float Weight)
{
	ArenaWeightSum -= Weight;
	if (WeightReducedDelegate.IsBound())
	{
		WeightReducedDelegate.Execute();
	}

	if (IsSpawnEnded && ArenaWeightSum <= 0)
	{
		IsArenaEnded = true;
		if (ArenaEndDelegate.IsBound())
		{
			ArenaEndDelegate.Broadcast();
		}
	}
}

float AArenaMaster::GetArenaWeight() const
{
	return ArenaWeightSum;
}

bool AArenaMaster::ArenaTicketsCheck(int TicketsMin) const 
{
	if (ArenaTickets >= TicketsMin)
	{
		return true;
	}

	return false;
}

// Called when the game starts or when spawned
void AArenaMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArenaMaster::SpawnStart()
{
	if (!IsArenaEnded)
	{
		if (ArenaStartDelegate.IsBound())
		{
			ArenaStartDelegate.Broadcast();
		}
		ArenaWeightSum = 0;

	
		if(SpawnComponentRef)
		{
			ISpawner::Execute_BeginSpawn(SpawnComponentRef, this);
		}
	}
}

void AArenaMaster::SpawnEnd()
{
	IsSpawnEnded = true;
}

void AArenaMaster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	TSet<UActorComponent*> Components = GetComponents();
	for (auto &Comp : Components)
	{
		if(Comp->GetClass()->ImplementsInterface(USpawner::StaticClass()))
		{
			SpawnComponentRef = Comp;
			break;
		}
	}

	//Spawn Enemy Target Points
	if (TargetPointsCount > TargetPoints.Num())
	{
		const FVector SpawnLocation = GetActorLocation();
		const FString NamePrefix = GetName() + "ETP_";
		int Counter = TargetPoints.Num();
		do
		{
			FString NewName = NamePrefix + FString::FromInt(Counter);
			Counter++;
			
			AEnemyTargetPoint *Tmp = GetWorld()->SpawnActor<AEnemyTargetPoint>(AEnemyTargetPoint::StaticClass());
			Tmp->SetActorLocation(SpawnLocation);
			//Tmp->Rename(ToCStr(NewName));
			TargetPoints.Add(Tmp);
		} while(TargetPointsCount != TargetPoints.Num());
	}

	if (TargetPointsCount < TargetPoints.Num())
	{
		do
		{
			TargetPoints.Top()->Destroy();
			TargetPoints.Pop();			
		} while(TargetPointsCount != TargetPoints.Num());
	}


	
	//Spawn Enemy Spawn points
	if (SpawnPointsCount > SpawnPoints.Num())
	{
		const FVector SpawnLocation = GetActorLocation();
		const FString NamePrefix = GetName() + "ETP_";
		int Counter = SpawnPoints.Num();
		do
		{
			FString NewName = NamePrefix + FString::FromInt(Counter);
			Counter++;
			
			AEnemySpawnPoint *Tmp = GetWorld()->SpawnActor<AEnemySpawnPoint>(AEnemySpawnPoint::StaticClass());
			Tmp->SetActorLocation(SpawnLocation);
			Tmp->SetArenaMaster(this);
			//Tmp->Rename(ToCStr(NewName));
			SpawnPoints.Add(Tmp);
		} while(SpawnPointsCount != SpawnPoints.Num());
	}

	if (SpawnPointsCount < SpawnPoints.Num())
	{
		do
		{
			SpawnPoints.Top()->Destroy();
			SpawnPoints.Pop();			
		} while(SpawnPointsCount != SpawnPoints.Num());
	}
}

void AArenaMaster::Reset()
{
	Super::Reset();

	for (auto &Enemy : SpawnedEnemies)
	{
		if (Enemy)
		{
			Enemy->Reset();
		}
	}
	SpawnedEnemies.Empty();

	ArenaWeightSum = 0;
	IsSpawnEnded = false;
}

