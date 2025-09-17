// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavLinkComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EnemyNavLauncher.generated.h"

class UNavLinkCustomComponent;

UCLASS()
class BLUNDERSGAME_API AEnemyNavLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyNavLauncher();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoxComponent *Launcher;

	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UNavLinkComponent *NavLinkComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget = ""))
	FVector TargetLocation;

	UFUNCTION()
	void OnLauncherOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void LaunchEnemy(class AEnemyParent *Enemy);
};
