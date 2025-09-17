// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/EnemyTargetPoint.h"
#include "DrawDebugHelpers.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyTargetPoint::AEnemyTargetPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	if (CollisionSphere)
	{
		SetRootComponent(CollisionSphere);
		CollisionSphere->SetCollisionProfileName(FName("Trigger"), true);
	}
	
#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite Component"));
	SpriteComponent->SetupAttachment(CollisionSphere);
#endif
}

// Called when the game starts or when spawned
void AEnemyTargetPoint::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AEnemyTargetPoint::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AEnemyTargetPoint::OnOverlapEnd);
}

void AEnemyTargetPoint::Reset()
{
	Super::Reset();

	CurrentEnemyWeight = FMath::RandRange(0, 50);
	EnemiesInTargetPoint.Empty();
}

void AEnemyTargetPoint::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AEnemyParent *Enemy = Cast<AEnemyParent>(OtherActor);
	if (Enemy)
	{
		AddEnemyWeight(Enemy);
	}
}

void AEnemyTargetPoint::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	AEnemyParent *Enemy = Cast<AEnemyParent>(OtherActor);
	if (Enemy)
	{
		DecreaseEnemyWeight(Enemy);
	}
}

void AEnemyTargetPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (CollisionSphere)
	{
		CollisionSphere->SetSphereRadius(PointRadius);
	}

	CurrentEnemyWeight = FMath::RandRange(0, 50);
}

float AEnemyTargetPoint::GetTotalWeight() const
{
	// @TODO Evaluate Target point Weight
	return CurrentEnemyWeight;
}

void AEnemyTargetPoint::AddEnemyWeight(AEnemyParent* Enemy)
{
	TWeakObjectPtr<AEnemyParent> EnemyRef(Enemy);
	if (EnemyRef.Get())
	{
		if (!EnemiesInTargetPoint.Contains(EnemyRef))
		{
			CurrentEnemyWeight += Enemy->GetEnemyWeight();
			EnemiesInTargetPoint.Add(EnemyRef);
		}
	}
}

void AEnemyTargetPoint::DecreaseEnemyWeight(AEnemyParent* Enemy)
{
	TWeakObjectPtr<AEnemyParent> EnemyRef(Enemy);
	if (EnemyRef.Get())
	{
		if (EnemiesInTargetPoint.Contains(EnemyRef))
		{
			CurrentEnemyWeight -= Enemy->GetEnemyWeight();
			EnemiesInTargetPoint.Remove(EnemyRef);
		}
	}
}

