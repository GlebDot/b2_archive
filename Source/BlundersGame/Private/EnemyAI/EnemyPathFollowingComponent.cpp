// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/EnemyPathFollowingComponent.h"

UEnemyPathFollowingComponent::UEnemyPathFollowingComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEnemyPathFollowingComponent::OnNavLaunch()
{
    SetNextMoveSegment();
}
