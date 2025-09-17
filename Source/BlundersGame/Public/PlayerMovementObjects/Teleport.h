// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Teleport.generated.h"

UCLASS()
class BLUNDERSGAME_API ATeleport : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleport();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent *SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TeleportSettings")
	UBoxComponent *TeleportCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TeleportSettings")
	UStaticMeshComponent *TeleportMesh;

	UFUNCTION(BlueprintCallable, Category="Teleportation")
	FVector GetTeleportationLocation(const float TeleportActorHalfHeight) const;

	UFUNCTION(BlueprintCallable)
	void SetTeleportsColor(const FLinearColor& NewColor)
	{
		TeleportColor = NewColor;
	}

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnTeleportOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportSettings")
	ATeleport *TeleportTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportSettings")
	float TeleportationLocationExtend = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportSettings")
	FLinearColor TeleportColor;

};
