// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlundersGame/Public/PlayerMovementStates/PlayerMovementState.h"
#include "PlayerSlideState.generated.h"
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerSlideState : public UPlayerMovementState
{
	GENERATED_BODY()
public:
    void OnChange() override;


    void OnExecute() override;
};
