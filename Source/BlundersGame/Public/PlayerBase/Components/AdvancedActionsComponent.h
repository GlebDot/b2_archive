// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerBase/MovementManager.h"
#include "UObject/Object.h"
#include "AdvancedActionsComponent.generated.h"

class APlayerCharacter;
class UAdvancedActionsComponent;
class UPlayerMovementState;

UENUM()
enum class EMovementStateSubtype : uint8
{
	Default UMETA(DisplayName="Default"),
	Empty UMETA(DisplayName="Empty"),
	Jump UMETA(DisplayName="Jump"),
	SlideJump UMETA(DisplayName="SlideJump"),
	WallJump UMETA(DisplayName="WallJump"),
	Dash UMETA(DisplayName="Dash"),
	FallDown UMETA(DisplayName="FallDown")
};

UCLASS()
class BLUNDERSGAME_API UMovementStateSummand : public UObject
{
	GENERATED_BODY()
public:
	UMovementStateSummand();

	EMovementStateSubtype Type;

	FORCEINLINE void SetAdvancedActionsComponent(UAdvancedActionsComponent *Component)
	{
		ActionsComponent = Component;
	}

	UFUNCTION()
	void StartExpire();
private:
	UFUNCTION()
	void ExpireEnd();
	
	UAdvancedActionsComponent *ActionsComponent;

	FTimerHandle TimeExpireTimerHandle;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UAdvancedActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAdvancedActionsComponent();

	UFUNCTION()
	void SetPlayerRef(APlayerCharacter *PlayerRef);

	UFUNCTION()
	void RemoveStateFromQueue(UMovementStateSummand *StateSummand);

	UFUNCTION()
	EPlayerMovementStates EvaluateQueue();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void MovementStateChanged(EMovementStateSubtype StateType);

	UFUNCTION()
	void ClearQueue();

	UFUNCTION()
	void AddStateToQueue(EMovementStateSubtype StateType);

	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY()
	TArray<UMovementStateSummand*> MovementStatesQueue;
};
