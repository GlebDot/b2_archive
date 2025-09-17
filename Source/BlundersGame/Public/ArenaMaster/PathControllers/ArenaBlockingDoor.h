// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaBlockingDoor.generated.h"

class AArenaMaster;

UCLASS()
class BLUNDERSGAME_API AArenaBlockingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaBlockingDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *DoorMesh;

protected:
	virtual void Reset() override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ArenaSettings");
	AArenaMaster *ArenaMasterRef;

	UFUNCTION()
	void OnArenaStart() const;

	UFUNCTION()
	void OnArenaEnd() const;
};
