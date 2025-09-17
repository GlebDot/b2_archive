// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

public:
    UEnemyPathFollowingComponent(const FObjectInitializer &ObjectInitializer);


    void OnNavLaunch();
};
