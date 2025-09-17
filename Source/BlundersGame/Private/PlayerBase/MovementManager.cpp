// Fill out your copyright notice in the Description page of Project Settings.

#include "BlundersGame/Public/PlayerBase/MovementManager.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Enemies/EnemyBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerMovement/PlayerMovementComponent.h"
#include "BlundersGame/Public/PlayerMovementStates/PlayerFallDownStates.h"
#include "BlundersGame/Private/PlayerBase/PlayerMovement/MovementStatesHelper.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBase/PlayerMovement/MomentumAccumulatorComponent.h"

//Return Energy remaining in 0-1 range
float UPlayerEnergyContainer::GetEnergyPercentage() const
{
	return CurrentEnergy / Params.ResourceEnergy;
}

void UPlayerEnergyContainer::InitControlledMeshMaterials() const
{
	ManagerRef->GetManagerMesh()->SetScalarParameterValueOnMaterials(Params.MaterialParameterControlName, 1.0);
}

void UPlayerEnergyContainer::RestoreEnergy()
{
	//Restore energy and clamp it to max Amount
	CurrentEnergy += Params.ResourceEnergyRecovery;
	CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.0f, Params.ResourceEnergy);

	//Set parameter on mesh for visual representation
	if (ManagerRef && ManagerRef->GetManagerMesh())
	{
		ManagerRef->GetManagerMesh()->SetScalarParameterValueOnMaterials(
			Params.MaterialParameterControlName,
			GetEnergyPercentage());
	}

	//Stop restore energy if max is achieved
	if (GetWorld() && CurrentEnergy == Params.ResourceEnergy)
	{
		GetWorld()->GetTimerManager().ClearTimer(TH_EnergyRestore);
	}
}

void UPlayerEnergyContainer::DecreaseEnergy(float EnergyAmount)
{
	//Decrease energy and clamp it's amount
	CurrentEnergy -= EnergyAmount;
	CurrentEnergy = FMath::Clamp(CurrentEnergy, 0.0f, Params.ResourceEnergy);

	//Set parameter on mesh for visual representation
	if (ManagerRef->GetManagerMesh())
	{		
		ManagerRef->GetManagerMesh()->SetScalarParameterValueOnMaterials(
			Params.MaterialParameterControlName,
			GetEnergyPercentage());
	}
	
	GetWorld()->GetTimerManager().ClearTimer(TH_EnergyCooldawn);
	GetWorld()->GetTimerManager().ClearTimer(TH_EnergyRestore);

	//Set cooldown timer
	GetWorld()->GetTimerManager().SetTimer(
		TH_EnergyCooldawn,
		this,
		&UPlayerEnergyContainer::EnergyRestoreCooldown,
		Params.ResourceEnergyRecoveryDelay,
		false);	
}

void UPlayerEnergyContainer::ConsumeAllEnergy()
{
	DecreaseEnergy(Params.ResourceEnergy);
}

bool UPlayerEnergyContainer::IsEnoughEnergy(float CheckValue)
{
	return CurrentEnergy >= CheckValue;
}

//Set all required parameters
void UPlayerEnergyContainer::Init(const FResourceEnergyData& InParams)
{
	CurrentEnergy = InParams.ResourceEnergy;
	Params = InParams;
}


void UPlayerEnergyContainer::SetManagerRef(UMovementManager* Manager)
{
	ManagerRef = Manager;
}

UWorld* UPlayerEnergyContainer::GetWorld() const
{
	return WorldRef;
}

void UPlayerEnergyContainer::EnergyRestoreCooldown()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			TH_EnergyRestore,
			this,
			&UPlayerEnergyContainer::RestoreEnergy,
			Params.ResourceEnergyRecoveryRate,
			true);
	}
}

UMovementManager::UMovementManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	MovementEnergyContainer = CreateDefaultSubobject<UPlayerEnergyContainer>(TEXT("Action Energy"));
	SupplyEnergyContainer = CreateDefaultSubobject<UPlayerEnergyContainer>(TEXT("Special Energy"));
	ManagerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Manager Mesh"));
}

void UMovementManager::Init(UMomentumAccumulatorComponent* InMomentumAccumulator, UPlayerMovementComponent* InMovementComponent, APlayerCharacter* InPlayer)
{
	MomentumAccumulator = InMomentumAccumulator;
	PlayerMovementComponent = InMovementComponent;
	Player = InPlayer;
	if (IsValid(MovementParams))
	{
		MomentumAccumulator->Init(MovementParams->MomentumParams);
	}
	InitMovementStates();
}


void UMovementManager::BeginPlay()
{
	Super::BeginPlay();
	
	check(IsValid(MovementParams));
	InitMovementParameters();
}

void UMovementManager::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	
	ClearMovementStates();
}

void UMovementManager::OnPlayerJumpedEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyBaseCharacter *JumpedEnemy = Cast<AEnemyBaseCharacter>(OtherActor);
	if (JumpedEnemy && Player)
	{
		const float PlayerZLocation = Player->GetActorLocation().Z;
		const float EnemyZLocation = JumpedEnemy->GetActorLocation().Z + JumpedEnemy->GetSimpleCollisionHalfHeight();
	
		if (PlayerZLocation >= EnemyZLocation)
		{
			const EEnemyJumpResponse Response = JumpedEnemy->PlayerJumpedOnEnemy();
			switch (Response)
			{
				case EEnemyJumpResponse::Jump:
					Player->SetCurrentState(GetMovementState(EPlayerMovementStates::JumpEnemy));
					break;
				case EEnemyJumpResponse::NoJump:
					break;
				case EEnemyJumpResponse::Trap:
					break;
			}
		}
	}
}

UPlayerMovementState* UMovementManager::GetMovementState(EPlayerMovementStates MovementStateType)
{
	return *MovementStatesMap.Find(MovementStateType);
}

void UMovementManager::InitMovementStates()
{
	FMovementStatesInitializer::InitMovementStatesMap(MovementStatesMap, Player, MovementParams->MovementSetParams);
	for (const TTuple<EPlayerMovementStates, UPlayerMovementState*>& MovementState : MovementStatesMap)
	{
		UPlayerMovementState* State = MovementState.Value;
		if (IsValid(State))
		{
			State->Init(MomentumAccumulator, this, Player, PlayerMovementComponent);
		}
	}
}

void UMovementManager::ClearMovementStates()
{
	for (TTuple<EPlayerMovementStates, UPlayerMovementState*>& MovementState : MovementStatesMap)
	{
		if ( IsValid(MovementState.Value) )
		{
			MovementState.Value->ConditionalBeginDestroy();
		}
	}

	MovementStatesMap.Empty();
}

void UMovementManager::UpdateMovementSpeed() const
{
	if (IsValid(MomentumAccumulator) && IsValid(PlayerMovementComponent))
	{
		const float SpeedMultiplier = 1 + MovementParams->SpeedParams.AdditionalMovementSpeedModifier *
			MomentumAccumulator->GetAdditionalMagnitudeRatioUnclamped();
		const float NewMaxMovementSpeed = MovementParams->SpeedParams.MovementSpeed * SpeedMultiplier;
		PlayerMovementComponent->MaxWalkSpeed = NewMaxMovementSpeed;
	}
}

void UMovementManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(PlayerMovementComponent->MaxWalkSpeed), true, false, FLinearColor::Red, 0.0f);
	UpdateMovementSpeed();
}

void UMovementManager::OnComponentCreated()
{
	Super::OnComponentCreated();
	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if ( IsValid(Character) && IsValid(MovementParams))
	{
		ManagerMesh->SetStaticMesh(MovementParams->BackPackMesh);
		ManagerMesh->AttachToComponent(
			Character->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			MovementParams->EnergyBagSocketName);
	}
}


void UMovementManager::ResetDash()
{
	CanDash = true;
}

void UMovementManager::InitMovementParameters()
{
	//Setting Energy containers parameters
	if (IsValid(MovementEnergyContainer))
	{
		MovementEnergyContainer->SetManagerRef(this);
		MovementEnergyContainer->SetWorldRef(GetWorld());
		MovementEnergyContainer->Init(MovementParams->MovementEnergyParams);
		MovementEnergyContainer->InitControlledMeshMaterials();
	}

	if (IsValid(SupplyEnergyContainer))
	{
		SupplyEnergyContainer->SetManagerRef(this);
		SupplyEnergyContainer->SetWorldRef(GetWorld());
		SupplyEnergyContainer->Init(MovementParams->SupplyEnergyParams);
		SupplyEnergyContainer->InitControlledMeshMaterials();
	}
	
	if (IsValid(PlayerMovementComponent))
	{
		PlayerMovementComponent->MaxWalkSpeed = MovementParams->SpeedParams.MovementSpeed;
	}
	if (IsValid(Player))
	{
		Player->MovementTriggerBottom->OnComponentBeginOverlap.AddUniqueDynamic(this, &UMovementManager::OnPlayerJumpedEnemy);
	}
	CurrentMovementSpeed = MovementParams->SpeedParams.MovementSpeed;
}

void UMovementManager::ResetJumps()
{
	JumpCountCurrent = MovementParams->JumpCount;
	CanWallJump = true;
	ManagerMesh->SetScalarParameterValueOnMaterials("JE_switch_1", 1.0);
	ManagerMesh->SetScalarParameterValueOnMaterials("JE_switch_2", 1.0);
}

//Jump check, may be should be redone
bool UMovementManager::JumpCheck(JumpTypes JumpType)
{
	//if jumping on the ground
	if (JumpType == JumpTypes::Ground)
	{
		--JumpCountCurrent;
		ManagerMesh->SetScalarParameterValueOnMaterials("JE_switch_1", 0.0);
		return true;
	}

	if (JumpType == JumpTypes::Wall && CanWallJump)
	{
		CanWallJump = false;
		return true;
	}

	//TODO: this is not right, however, I don't know who should be responsible for spending movement energy
	UPlayerMovementState* const& JumpState = MovementStatesMap.FindChecked(EPlayerMovementStates::JumpAir);
	const FMovementStateData& StateData = JumpState->GetStateParams();
	{
		MovementEnergyContainer->DecreaseEnergy(StateData.StateEnergyCost);
		--JumpCountCurrent;
		switch (JumpCountCurrent)
		{
			//TODO: use custom data for this 
			case 1:
				ManagerMesh->SetScalarParameterValueOnMaterials("JE_switch_1", 0.0);
				break;
			case 0:
				ManagerMesh->SetScalarParameterValueOnMaterials("JE_switch_2", 0.0);
				break;
			default:
				break;
		}
		return true;
	}

	return false;
}

bool UMovementManager::DashCheck()
{
	//TODO: this is not right, however, I don't know who should be responsible for spending movement energy
	UPlayerMovementState* const& DashState = MovementStatesMap.FindChecked(EPlayerMovementStates::DashBase);
	const FMovementStateData& StateData = DashState->GetStateParams();
	
	if (CanDash && MovementEnergyContainer->IsEnoughEnergy(StateData.StateEnergyCost))
	{
		MovementEnergyContainer->DecreaseEnergy(StateData.StateEnergyCost);
		CanDash = false;
		GetWorld()->GetTimerManager().SetTimer(
			TH_Dash,
			this,
			&UMovementManager::ResetDash,
			StateData.StateCooldown,
			false);
		
		return true;		
	}

	return false;
}

bool UMovementManager::SpecialMoveCheck() const
{
	if (SupplyEnergyContainer->GetEnergyPercentage() == 1.0f) 
	{
		SupplyEnergyContainer->ConsumeAllEnergy();
		return true;
	}

	return false;
}
