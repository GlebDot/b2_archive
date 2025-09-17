// Fill out your copyright notice in the Description page of Project Settings.
#include "BlundersGame/Public/PlayerBase/UI/PlayerHitNotificationUserWidget.h"

#include "UObject/UnrealTypePrivate.h"

UPlayerHitNotificationUserWidget::UPlayerHitNotificationUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHitNotificationUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    HitAnimation = GetAnimationByName(FName("HitNotification"));
}

void UPlayerHitNotificationUserWidget::OnPlayerHit()
{
    if (HitAnimation)
    {
        PlayAnimation(HitAnimation, 0, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
    }
}


