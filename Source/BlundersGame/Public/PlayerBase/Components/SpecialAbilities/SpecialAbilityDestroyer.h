// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "SpecialAbilityComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SpecialAbilityDestroyer.generated.h"

UCLASS()
class BLUNDERSGAME_API ASpecialAbilityDestroyer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpecialAbilityDestroyer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent *BoxCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SpecialAbilityType")
	TSubclassOf<USpecialAbilityComponent> SpecialAbilityType;

	virtual void Destroyed() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
