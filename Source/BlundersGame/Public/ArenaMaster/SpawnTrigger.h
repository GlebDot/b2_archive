// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnTrigger.generated.h"

UCLASS()
class BLUNDERSGAME_API ASpawnTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Reset() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Properties")
	class UBoxComponent *BoxTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger Properties")
	class AArenaMaster *ArenaMasterRef;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
				   AActor* OtherActor,
				   UPrimitiveComponent* OtherComp,
				   int32 OtherBodyIndex,
				   bool bFromSweep,
				   const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawn")
	void SpawnBegin();

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Display)
	class UBillboardComponent* SpriteComponent;
#endif

};
