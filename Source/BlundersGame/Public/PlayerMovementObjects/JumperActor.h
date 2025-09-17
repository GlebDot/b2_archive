// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "JumperActor.generated.h"

class APlayerCharacter;

UCLASS()
class BLUNDERSGAME_API AJumperActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumperActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumper Settings")
	UStaticMeshComponent *Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumper Settings")
	UBoxComponent *JumperCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jumper Settings")
	float JumpVelocity = 1000.0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LaunchPlayer(APlayerCharacter *Character, const FVector& LaunchVelocity);

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY()
	bool CanLaunch = true;
	
private:
	UFUNCTION()
	void ResetJumper();
	
	UPROPERTY()
	FTimerHandle JumperResetTimerHandle;

	UPROPERTY()
	float CooldownTime = 0.5f;
};
