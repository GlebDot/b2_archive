// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementStates/PlayerMovementState.h"
#include "PlayerFallDownStates.generated.h"

class UCharacterMovementComponent;

DECLARE_DELEGATE(FOnFallDownComplete);
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerFallDownState : public UPlayerMovementState
{
	GENERATED_BODY()


public:
	FOnFallDownComplete OnFallDownComplete;
	UPlayerFallDownState();
	
	virtual void OnExecute() override;
	virtual void OnChange() override;
	virtual void OnLanding() override;
	
private:
	UPROPERTY()
	float GravityScaleSave;
};
