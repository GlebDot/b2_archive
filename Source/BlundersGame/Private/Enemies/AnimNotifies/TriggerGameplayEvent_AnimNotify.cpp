// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimNotifies/TriggerGameplayEvent_AnimNotify.h"

#include "AbilitySystemBlueprintLibrary.h"

void UTriggerGameplayEvent_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AActor *Owner = MeshComp->GetOwner();
    FGameplayEventData EventData = FGameplayEventData();
    EventData.EventTag = EventTag;
    EventData.Instigator = Owner;
    if (Owner)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, EventData);
    }
}
