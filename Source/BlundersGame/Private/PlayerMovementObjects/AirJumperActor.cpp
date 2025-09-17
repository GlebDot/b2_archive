// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerMovementObjects/AirJumperActor.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void AAirJumperActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (CanLaunch)
    {
        APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor);
        if (Player)
        {
            FVector PlayerVelocityVector = Player->GetVelocity();
            PlayerVelocityVector.Normalize();
            const float VectorCos = UKismetMathLibrary::Dot_VectorVector(PlayerVelocityVector, GetActorUpVector());

            if (VectorCos > 0.0)
            {
                const FVector LaunchVector = GetActorUpVector() * JumpVelocity;
                LaunchPlayer(Player, LaunchVector);
            }
        }
    }
}
