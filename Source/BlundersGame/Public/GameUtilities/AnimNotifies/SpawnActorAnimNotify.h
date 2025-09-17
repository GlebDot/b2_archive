// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SpawnActorAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API USpawnActorAnimNotify : public UAnimNotify
{
	GENERATED_BODY()


public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Settings")
	FName SocketName;
};
