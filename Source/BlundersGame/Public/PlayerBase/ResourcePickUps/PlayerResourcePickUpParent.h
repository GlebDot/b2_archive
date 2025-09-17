// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "PlayerResourcePickUpParent.generated.h"

class APlayerCharacter;

UCLASS()
class BLUNDERSGAME_API APlayerResourcePickUpParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerResourcePickUpParent();

	virtual void Reset() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent *SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent *PickUpMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FCollisionObjectQueryParams CollisionObjectQueryParams;

	UFUNCTION()
	virtual void TransferResources(APlayerCharacter *Player) {};

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent,
							  AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex,
							  bool bFromSweep,
							  const FHitResult & SweepResult);

	UFUNCTION()
	bool PlayerPickCheck(AActor* OtherActor);

	UFUNCTION()
	virtual void OverlapCheckOnSpawn();

	UPROPERTY()
	FTimerHandle TH_OverlapCheckSpawn;
};
