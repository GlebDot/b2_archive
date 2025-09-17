// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/Components/AdvancedActionsComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"

UMovementStateSummand::UMovementStateSummand()
{
	
}

void UMovementStateSummand::StartExpire()
{
	if (ActionsComponent)
	{
		ActionsComponent->GetWorld()->GetTimerManager().SetTimer(TimeExpireTimerHandle, this, &UMovementStateSummand::ExpireEnd, 0.2f, false);
	}
}

void UMovementStateSummand::ExpireEnd()
{
	if (ActionsComponent)
	{
		ActionsComponent->RemoveStateFromQueue(this);
	}
}

// Sets default values for this component's properties
UAdvancedActionsComponent::UAdvancedActionsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAdvancedActionsComponent::SetPlayerRef(APlayerCharacter* PlayerRef)
{
	Player = PlayerRef;
}


// Called when the game starts
void UAdvancedActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	Player->OnPlayerMovementStateChanged.BindUFunction(this, FName("MovementStateChanged"));
}

void UAdvancedActionsComponent::MovementStateChanged(EMovementStateSubtype StateType)
{
	switch (StateType)
	{
		case EMovementStateSubtype::Empty:
			break;
		case EMovementStateSubtype::Default:
			ClearQueue();
			break;
		default:
			AddStateToQueue(StateType);
			break;
	}
}

void UAdvancedActionsComponent::ClearQueue()
{
	MovementStatesQueue.Empty();
}

void UAdvancedActionsComponent::AddStateToQueue(EMovementStateSubtype StateType)
{
	UMovementStateSummand *StateSummand = NewObject<UMovementStateSummand>();
	StateSummand->Type = StateType;
	StateSummand->SetAdvancedActionsComponent(this);
	StateSummand->StartExpire();
	MovementStatesQueue.Add(StateSummand);
}

void UAdvancedActionsComponent::RemoveStateFromQueue(UMovementStateSummand *StateSummand)
{
	MovementStatesQueue.Remove(StateSummand);	
}

EPlayerMovementStates UAdvancedActionsComponent::EvaluateQueue()
{
	if (MovementStatesQueue.Num() > 1)
	{
		if (MovementStatesQueue[MovementStatesQueue.Num() - 2]->Type == EMovementStateSubtype::Dash &&
			MovementStatesQueue[MovementStatesQueue.Num() - 1]->Type == EMovementStateSubtype::Jump)
		{
			if (Player->GetMovementInputForward() > FMath::Abs(Player->GetMovementInputRight()))
			{
				ClearQueue();
				// UPlayerJumpStateLong *JumpStateLong = NewObject<UPlayerJumpStateLong>();
				return EPlayerMovementStates::JumpLong;
			}
		}

		if (MovementStatesQueue[MovementStatesQueue.Num() - 1]->Type == EMovementStateSubtype::Dash &&
            MovementStatesQueue[MovementStatesQueue.Num() - 2]->Type == EMovementStateSubtype::Jump)
		{
			ClearQueue();
			// UPlayerDashStateVertical *DashStateVertical = NewObject<UPlayerDashStateVertical>();
			return EPlayerMovementStates::DashVertical;
		}

		if (MovementStatesQueue[MovementStatesQueue.Num() - 1]->Type == EMovementStateSubtype::Dash &&
            MovementStatesQueue[MovementStatesQueue.Num() - 2]->Type == EMovementStateSubtype::WallJump)
		{
			ClearQueue();
			// UPlayerJumpStateBackward *JumpStateBackward = NewObject<UPlayerJumpStateBackward>();
			return EPlayerMovementStates::JumpBack;
		}

		if (MovementStatesQueue[MovementStatesQueue.Num() - 1]->Type == EMovementStateSubtype::Dash &&
            MovementStatesQueue[MovementStatesQueue.Num() - 2]->Type == EMovementStateSubtype::SlideJump)
		{
			ClearQueue();
			// UPlayerDashStateLong *LongDashState = NewObject<UPlayerDashStateLong>();
			return EPlayerMovementStates::DashLong;
		}

		if (MovementStatesQueue[MovementStatesQueue.Num() - 1]->Type == EMovementStateSubtype::Jump &&
            MovementStatesQueue[MovementStatesQueue.Num() - 2]->Type == EMovementStateSubtype::FallDown)
		{
			ClearQueue();
			// UPlayerJumpStateHigh *HighJumpState = NewObject<UPlayerJumpStateHigh>();
			return EPlayerMovementStates::JumpHigh;
		}
	}

	return EPlayerMovementStates::Default;
}


