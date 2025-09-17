// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerBase/PlayerMovement/CharacterMovementParams.h"
#include "MovementManager.generated.h"

class UMovementManager;
class APlayerCharacter;
class UPlayerMovementState;
class UPlayerJumpStateEnemy;
class UMomentumAccumulatorComponent;
class UPlayerMovementComponent;

UENUM(BlueprintType)
enum class EPlayerMovementStates : uint8
{
	Default UMETA(DiaplayName = "Default"),
	
	// JUMPS
	JumpGround UMETA(DisplayName = "GroundJump"),
    JumpAir UMETA(DisplayName = "AirJump"),
    JumpWall UMETA(DisplayName = "WallJump"),
    JumpLong UMETA(DisplayName = "LongJump"),
    JumpHigh UMETA(DisplayName = "HighJump"),
    JumpJumper UMETA(DisplayName = "JumperJump"),
    JumpBack UMETA(DisplayName = "BackJump"),
    JumpSlider UMETA(DisplayName = "SlidingJump"),
	JumpEnemy UMETA(DisplayName = "EnemyJump"),

	// DASHES
	DashBase UMETA(DisplayName = "Dash"),
    DashVertical UMETA(DisplayName = "VerticalDash"),
    DashLong UMETA(DisplayName = "LongDash"),

	// FALLING
	DropDown UMETA(DisplayName = "FallingDown"),

	//ATTACKS
	AirAttack UMETA(DisplayName = "AirAttack"),
	GroundAttack UMETA(DisplayName = "GroundAttack"),

	//SLIDING
	Slide UMETA(DisplayName = "Slide")
};


UENUM()
enum JumpTypes
{
	Ground UMETA(DisplayName = "Ground"),
	Air UMETA(DisplayName = "Air"),
	Wall UMETA(DisplayName = "Wall"),
};

UCLASS()
class BLUNDERSGAME_API UPlayerEnergyContainer : public UObject
{
	GENERATED_BODY()
public:
	UPlayerEnergyContainer() = default;
	
	float GetEnergyPercentage() const;
	void InitControlledMeshMaterials() const;
	void RestoreEnergy();
	void DecreaseEnergy(float EnergyAmount);
	void ConsumeAllEnergy();
	bool IsEnoughEnergy(float CheckValue);
	void Init(const FResourceEnergyData& InParams);

	void SetWorldRef(UWorld *World)
	{
		WorldRef = World;
	}

	void SetManagerRef(UMovementManager *Manager);

	virtual UWorld* GetWorld() const override;
protected:
	void EnergyRestoreCooldown();

	UPROPERTY()
	FResourceEnergyData Params {};
	float CurrentEnergy;

	FTimerHandle TH_EnergyCooldawn;
	FTimerHandle TH_EnergyRestore;

	UPROPERTY(Transient)
	UWorld *WorldRef;

	UPROPERTY()
	UMovementManager *ManagerRef;
};

//Class for managing and storing different energy types
//ALso It's responsible for player's movement abilities
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UMovementManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="MovementParams")
	UCharacterMovementParams* MovementParams;
	
	// Sets default values for this component's properties
	UMovementManager();
	void Init(UMomentumAccumulatorComponent* InMomentumAccumulator, UPlayerMovementComponent* InMovementComponent, APlayerCharacter* InPlayer);
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentCreated() override;

	void ResetJumps();

	void ResetDash();

	void InitMovementParameters();

	bool GetCanWallJump() const
	{
		return CanWallJump;
	}

	float GetMovementSpeed() const
	{
		return CurrentMovementSpeed;
	}

	UStaticMeshComponent *GetManagerMesh()
	{
		return ManagerMesh;
	}

	float GetActionEnergyPercent() const
	{
		return MovementEnergyContainer->GetEnergyPercentage();
	}

	//Checking different types of movement abilities
	bool JumpCheck(JumpTypes JumpType);
	bool DashCheck();
	bool SpecialMoveCheck() const;

	//Behaviour functions
	//void ConsumeJump();
protected:
	UPROPERTY()
	UPlayerEnergyContainer *MovementEnergyContainer;

	UPROPERTY()
	UPlayerEnergyContainer *SupplyEnergyContainer;

	UPROPERTY()
	UMomentumAccumulatorComponent *MomentumAccumulator;

	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UFUNCTION()
	void OnPlayerJumpedEnemy(UPrimitiveComponent* OverlappedComponent,
							 AActor* OtherActor,
							 UPrimitiveComponent* OtherComp,
							 int32 OtherBodyIndex,
							 bool bFromSweep,
							 const FHitResult &SweepResult);

	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY()
	UStaticMeshComponent *ManagerMesh;

	UPROPERTY()
	UPlayerMovementComponent* PlayerMovementComponent;

public:
	UFUNCTION()
	UPlayerMovementState* GetMovementState(EPlayerMovementStates MovementStateType);

private:	
	void InitMovementStates();

	UFUNCTION()
	void ClearMovementStates();

	void UpdateMovementSpeed() const;
	
	UPROPERTY()
	TMap<EPlayerMovementStates, UPlayerMovementState*> MovementStatesMap;

	int JumpCountCurrent = 0;
	float CurrentMovementSpeed = 900.0f;
	
	bool CanDash = true;
	bool CanWallJump = true;
	FTimerHandle TH_Dash {};
};
