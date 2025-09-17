// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlundersGameModeBase.generated.h"

class ACheckPointActor;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API ABlundersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void OnPlayerFellOut();
    
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void OnPlayerDead();
    
    ABlundersGameModeBase(const FObjectInitializer &ObjectInitializer);

    virtual void ResetLevel() override;

    UFUNCTION()
    void RestartPlayerAfterDeath();
    
    UFUNCTION()
    void SetCurrentCheckPoint(ACheckPointActor *CheckPoint);

    UFUNCTION()
    void RestartPlayerAfterFalloff(APawn *Player);
protected:
    UPROPERTY(BlueprintReadWrite, Category="GameSave");
    ACheckPointActor *CurrentCheckPoint;
};
