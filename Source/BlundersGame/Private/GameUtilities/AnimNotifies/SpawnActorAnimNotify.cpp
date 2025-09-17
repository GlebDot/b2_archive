// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/GameUtilities/AnimNotifies/SpawnActorAnimNotify.h"

void USpawnActorAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    const FVector SpawnLocation = MeshComp->GetSocketLocation(SocketName);
    AActor *SpawnedActor = MeshComp->GetWorld()->SpawnActor(ActorToSpawn, &SpawnLocation);
    if (SpawnedActor)
    {
        SpawnedActor->SetInstigator(Cast<APawn>(MeshComp->GetOwner()));
    }
}
