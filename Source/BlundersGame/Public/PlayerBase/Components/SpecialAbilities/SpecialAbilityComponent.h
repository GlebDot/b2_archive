// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpecialAbilityComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API USpecialAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpecialAbilityComponent();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	void DetachSpecialAbilityComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
    virtual void SpecialAbilityUsage() {};

	APlayerCharacter *Player;
		
};
