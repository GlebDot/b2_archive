// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/AnimNotifyState_EnemyJump.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotifyState_EnemyJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        ACharacter *Character = Cast<ACharacter>(MeshComp->GetOwner());

        if (Character)
        {
            Character->LaunchCharacter(FVector(0.0f, 0.0f, JumpVelocity), false, true);
            if (IsEnemyHanging)
            {
                UCharacterMovementComponent *MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
                MovementComponent->GravityScale = 0.0;
            }
        }
    }
}

void UAnimNotifyState_EnemyJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (IsEnemyHanging && MeshComp && MeshComp->GetOwner())
    {
        ACharacter *Character = Cast<ACharacter>(MeshComp->GetOwner());
        if (Character)
        {
            UCharacterMovementComponent *MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
            MovementComponent->GravityScale = GravityRestoreValue;
        }
    }
}
