// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/Components/SpecialAbilities/SpecialAbilityComponent.h"
#include "Swaper_SpecialAbilityComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLUNDERSGAME_API USwaper_SpecialAbilityComponent : public USpecialAbilityComponent
{
	GENERATED_BODY()

public:
	USwaper_SpecialAbilityComponent();
	
protected:
	virtual void SpecialAbilityUsage() override;

	virtual void BeginPlay() override;

private:
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	FCollisionQueryParams CollisionQueryParams;
};
