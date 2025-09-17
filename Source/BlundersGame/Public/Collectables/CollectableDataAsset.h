// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CollectableDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BLUNDERSGAME_API UCollectableDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Base")
	FString Info;
};
