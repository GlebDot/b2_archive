// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerParentUserWidget.h"
#include "Blueprint/UserWidget.h"

#include "PlayerHitNotificationUserWidget.generated.h"
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerHitNotificationUserWidget : public UPlayerParentUserWidget
{
	GENERATED_BODY()

public:
    UPlayerHitNotificationUserWidget(const FObjectInitializer &ObjectInitializer);

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnPlayerHit();

private:
    UPROPERTY()
    UWidgetAnimation *HitAnimation;
};
