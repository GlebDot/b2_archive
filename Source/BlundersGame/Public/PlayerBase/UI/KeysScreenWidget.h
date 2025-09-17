// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/UI/PlayerParentUserWidget.h"
#include "KeysScreenWidget.generated.h"


class UKeyHolderComponent;

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UKeysScreenWidget : public UPlayerParentUserWidget
{
	GENERATED_BODY()

public:
	UKeysScreenWidget(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetKeyHolder(UKeyHolderComponent *KeyHolder) { KeyHolderComponentRef = KeyHolder; }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateInfo();

protected:
	UPROPERTY(BlueprintReadWrite)
	UKeyHolderComponent *KeyHolderComponentRef;
	
};
