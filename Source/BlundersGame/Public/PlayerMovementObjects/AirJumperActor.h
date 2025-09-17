// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementObjects/JumperActor.h"
#include "AirJumperActor.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API AAirJumperActor : public AJumperActor
{
	GENERATED_BODY()


protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
