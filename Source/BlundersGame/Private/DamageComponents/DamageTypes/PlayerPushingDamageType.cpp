// Fill out your copyright notice in the Description page of Project Settings.
#include "BlundersGame/Public/DamageComponents/DamageTypes/PlayerPushingDamageType.h"


#include "Enemies/EnemyBaseCharacter.h"
#include "EnemyAI/EnemyParentAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerBase/PlayerCharacter.h"

float UPlayerPushingDamageType::ProcessDamage_Implementation(float BaseDamage, AActor* DamagedActor) const
{
    if (DamagedActor->ActorHasTag("Player"))
    {
        return 0.0f;
    }

    AEnemyBaseCharacter *Enemy = Cast<AEnemyBaseCharacter>(DamagedActor);
    ACharacter *Player = UGameplayStatics::GetPlayerCharacter(DamagedActor->GetWorld(), 0);
    if (Enemy && Player)
    {
        FVector StartLocation = Player->GetActorLocation();
        StartLocation.Z = 0.0f;
        FVector EndLocation = Enemy->GetActorLocation();
        EndLocation.Z = 0.0f;

        FVector PushDirection = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation).Vector();
        PushDirection = PushDirection + Enemy->GetActorUpVector();
        PushDirection.Normalize();

        Enemy->GetMesh()->GetAnimInstance()->Montage_Stop(0.2);
        Enemy->LaunchCharacter(PushDirection * 850.0f, true, true);
        AEnemyParentAIController *EnemyController = Cast<AEnemyParentAIController>(Enemy->GetController());
        if (EnemyController)
        {
            EnemyController->EnemyStun(1.75f);
        }
    }

    return BaseDamage;
}
