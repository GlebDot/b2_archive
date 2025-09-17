// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CheckPointActor.generated.h"

UCLASS()
class BLUNDERSGAME_API ACheckPointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPointActor();

	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CheckPoint")
	USceneComponent *SceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CheckPoint")
	UBoxComponent *BoxCollision;

	UFUNCTION()
	void OnCheckPointBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
