// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaMaster/ArenaMaster.h"
#include "Components/ActorComponent.h"
#include "SpawnParentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API USpawnParentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnParentComponent();

protected:
	UPROPERTY()
	AArenaMaster *ArenaRef;

	UFUNCTION()
	void SpawnEnd();
		
};
