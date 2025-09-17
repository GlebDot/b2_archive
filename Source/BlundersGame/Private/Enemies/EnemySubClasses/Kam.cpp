// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/EnemySubClasses/Kam.h"


#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKam::AKam()
{
    this->bAllowTickBeforeBeginPlay = false;
    this->SetActorTickEnabled(false);
    
    HintMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hint Mesh"));
    HintMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMesh(TEXT("StaticMesh'/Niagara/DefaultAssets/S_Arrow.S_Arrow'"));
    if (ArrowMesh.Succeeded())
    {
        HintMesh->SetStaticMesh(ArrowMesh.Object);
    }

}

void AKam::OnActorDestroyed(AActor* DestroyedActor)
{
    Super::OnActorDestroyed(DestroyedActor);
    const FVector SpawnLocation = GetActorLocation() + GetActorUpVector() * 75.0f;
    GetWorld()->SpawnActor(ExplosionType, &SpawnLocation);
}

void AKam::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    HintMesh->SetVisibility(false);

    HintMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    HintMesh->SetWorldScale3D(FVector(5.0f));
}

void AKam::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    CalculateDistanceToPlayer();
}

void AKam::BeginPlay()
{
    Super::BeginPlay();

    FTimerDelegate TD_TickEnable;
    FTimerHandle TH_TickEnable;
    TD_TickEnable.BindUFunction(this, FName("SetActorTickEnabled"), true);
    GetWorld()->GetTimerManager().SetTimer(TH_TickEnable, TD_TickEnable, 0.3f, false);
}

void AKam::CalculateDistanceToPlayer()
{
    if (PlayerRef)
    {
        const float Distance = UKismetMathLibrary::Vector_Distance(PlayerRef->GetActorLocation(), GetActorLocation());

        if (Distance < ExplosionDistance * 2.5f)
        {
            if (!IsHintOnPlayer)
            {
                IsHintOnPlayer = true;
                HintMesh->SetVisibility(true);
                HintMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
                HintMesh->AttachToComponent(PlayerRef->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            }
            const FRotator HintRotation = UKismetMathLibrary::FindLookAtRotation(PlayerRef->GetActorLocation(), GetActorLocation());
            HintMesh->SetWorldRotation(HintRotation);
        } else
        {
            if (IsHintOnPlayer)
            {
                IsHintOnPlayer = false;
                HintMesh->SetVisibility(false);
            }
        }

        if (Distance < ExplosionDistance)
        {
            AAIController *Con = Cast<AAIController>(GetController());
            if (Con->LineOfSightTo(PlayerRef))
            {
                this->Destroy();
            }
        }
    }
}
