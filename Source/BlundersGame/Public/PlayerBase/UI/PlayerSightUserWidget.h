// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerParentUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

#include "PlayerSightUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerSightUserWidget : public UPlayerParentUserWidget
{
	GENERATED_BODY()
public:
    UPlayerSightUserWidget(const FObjectInitializer &ObjectInitializer);

    virtual void NativeConstruct() override;

    UFUNCTION()
    void EnemyDamaged();

    UFUNCTION()
    void EnemyKilled();

    UFUNCTION()
    void SetLeftAmmoPercent(const float Percent) const;

    UFUNCTION()
    void SetRightAmmoPercent(const float Percent) const;

    UFUNCTION()
    UProgressBar *GetProgressBarByName(const FName ProgressBarName) const;

protected:
    UFUNCTION()
    void StoreProgressBars();

    UPROPERTY()
    TMap<FName, UProgressBar*> ProgressBarsMap;

    UPROPERTY()
    UProgressBar *AmmoLeft_PB;

    UPROPERTY()
    UProgressBar *AmmoRight_PB;
    
    UPROPERTY()
    UWidgetAnimation *EnemyHitAnimation;

    UPROPERTY()
    UWidgetAnimation *EnemyDeathAnimation;
};
