// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/Explosions/ExplosionBasic.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AExplosionBasic::AExplosionBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}

	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_EngineTraceChannel1); //EnemyChannel
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
}

// Called when the game starts or when spawned
void AExplosionBasic::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TH_Explosion, this, &AExplosionBasic::Explosion, 0.1f, false);
}

void AExplosionBasic::Explosion()
{

	GetWorld()->GetTimerManager().ClearTimer(TH_Explosion);
	SetLifeSpan(1.0);
	ExplosionShape.SetSphere(DamageRadius);
	
	//Only for debug
	//This should be particle system
	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 12, FColor::Red, false, 1.0f, 0, 1);

	TArray<FHitResult> HitResults;
	const FVector ExplosionCenter = GetActorLocation();
	
	GetWorld()->SweepMultiByObjectType(HitResults, ExplosionCenter, ExplosionCenter, FQuat::Identity,
									   CollisionObjectQueryParams, ExplosionShape, CollisionQueryParams);

	TArray<AActor*> DamagedActors; 
	for (FHitResult &HitResult : HitResults)
	{
		AActor *PendingDamageActor = HitResult.GetActor();
		if (PendingDamageActor && !DamagedActors.Contains(PendingDamageActor))
		{			
			FHitResult OutHit;
			const FVector CheckTraceDirection = UKismetMathLibrary::FindLookAtRotation(ExplosionCenter, PendingDamageActor->GetActorLocation()).Vector();
			const FVector CheckTraceStart = CheckTraceDirection * 100.0f + ExplosionCenter;
			const FVector CheckTraceEnd = CheckTraceDirection * DamageRadius + ExplosionCenter;
			
			const bool IsHit = GetWorld()->LineTraceSingleByObjectType(OutHit, CheckTraceStart, CheckTraceEnd, CollisionObjectQueryParams, CollisionQueryParams);
			if (IsHit)
			{
				if (OutHit.GetActor() == PendingDamageActor)
				{
					DamagedActors.AddUnique(PendingDamageActor);
					UGameplayStatics::ApplyDamage(PendingDamageActor, Damage, GetInstigatorController(), GetInstigator(), DamageTypeClass);
				}
			}
			
		} 
	}
	DamagedActors.Empty();
	// if (ExplosionFX)
	// {
	// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionFX, GetActorLocation());
	// }
}

