// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/UI/PlayerParentUserWidget.h"
#include "WeaponSocketUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UWeaponSocketUserWidget : public UPlayerParentUserWidget
{
	GENERATED_BODY()

public:
    UWeaponSocketUserWidget(const FObjectInitializer &ObjectInitializer);

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Player Weapon Info")
    void OnShot();
};
