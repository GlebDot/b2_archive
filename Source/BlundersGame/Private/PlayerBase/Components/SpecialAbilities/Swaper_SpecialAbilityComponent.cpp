// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/Components/SpecialAbilities/Swaper_SpecialAbilityComponent.h"

#include "DrawDebugHelpers.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "Enemies/EnemyBaseCharacter.h"

USwaper_SpecialAbilityComponent::USwaper_SpecialAbilityComponent()
{
    CollisionObjectQueryParams = FCollisionObjectQueryParams();
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
    CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1); //EnemyChannel

    CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
}

void USwaper_SpecialAbilityComponent::SpecialAbilityUsage()
{
    if (Player)
    {
        const FVector StartTrace = Player->Camera->GetComponentLocation();
        const FVector EndTrace = Player->Camera->GetForwardVector() * 10000.0f + StartTrace;
        FHitResult HitResult;
        const bool isHit = GetWorld()->LineTraceSingleByObjectType(HitResult, StartTrace, EndTrace, CollisionObjectQueryParams, CollisionQueryParams);

        if (isHit)
        {
            AEnemyBaseCharacter *Enemy = Cast<AEnemyBaseCharacter>(HitResult.GetActor());
            if (Enemy)
            {
                const float EnemyHalfHeight = Enemy->GetSimpleCollisionHalfHeight();
                const float PlayerHalfHeight = Player->GetSimpleCollisionHalfHeight();
                const FVector EnemyLocation = Enemy->GetActorLocation();
                const FVector NewEnemyLocation = Player->GetActorLocation() + FVector(0, 0, EnemyHalfHeight);
                Enemy->SetActorLocation(NewEnemyLocation);
                Player->SetActorLocation(EnemyLocation + FVector(0, 0, PlayerHalfHeight * 2.0f));

                // I don't sure about this, gameplay testing required
                Player->MovementManager->ResetJumps();

                DrawDebugLine(GetWorld(), StartTrace, HitResult.Location, FColor::Green, false, 2.0f, 0, 4.0f);
            } else
            {
                DrawDebugLine(GetWorld(), StartTrace, HitResult.Location, FColor::Red, false, 2.0f, 0, 4.0f);
            }
        } else
        {
            DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Black, false, 2.0f, 0, 4.0f);
        }
    }
}

void USwaper_SpecialAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    CollisionQueryParams.AddIgnoredActor(Player);
}
