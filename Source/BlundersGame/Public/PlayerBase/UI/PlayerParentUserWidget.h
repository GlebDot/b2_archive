// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerParentUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerParentUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UPlayerParentUserWidget(const FObjectInitializer &ObjectInitializer);

    virtual void NativeConstruct() override;

protected:
    UWidgetAnimation *GetAnimationByName(const FName AnimationName) const;

    UFUNCTION()
    void StoreWidgetAnimations();
private:
    UPROPERTY()
    TMap<FName, UWidgetAnimation*> AnimationsMap;
};
