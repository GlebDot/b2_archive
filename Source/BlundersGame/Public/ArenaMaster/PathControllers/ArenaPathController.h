// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaMaster/ArenaMaster.h"
#include "GameFramework/Actor.h"
#include "ArenaPathController.generated.h"

UCLASS()
class BLUNDERSGAME_API AArenaPathController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaPathController();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="ArenaControll")
	void ArenaEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="ArenaControll")
    void ArenaStarted();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Arena")
	AArenaMaster *ArenaRef;

};
