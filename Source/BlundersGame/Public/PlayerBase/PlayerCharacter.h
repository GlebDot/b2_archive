// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MovementManager.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "UObject/ObjectMacros.h"
#include "BlundersGame/Public/PlayerBase/Components/AdvancedActionsComponent.h"
#include "PlayerCharacter.generated.h"

class UWeaponryComponent;
class APlayerHUD;
class UPlayerMovementState;
class APlayerSlider;
class UWeaponBase;
class UArmourDamageHandlerComponent;
class UPlayerAmmoComponent;
class UPlayerSpeedEnergyComponent;
class UKeyHolderComponent;
class UPlayerFallDownState;
class UMomentumAccumulatorComponent;
class UPlayerMovementComponent;

//Enum for player states
//TODO: bad bad naming
UENUM(BlueprintType)
namespace PlayerMovementStates
{
	enum Type
	{
		Dash UMETA(DisplayName = "Dash"),
        Jump UMETA(DisplayName = "Jump"),
        WallJump UMETA(DisplayName = "WallJump"),
        Slide UMETA(DisplayName = "Slide"),
        Run UMETA(DisplayName = "Run"),
		FallDown UMETA(DisplayName = "FallDown")
    };
}


//Player class
UCLASS()
class BLUNDERSGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnPlayerMovementStateChanged, EMovementStateSubtype)
	FOnPlayerMovementStateChanged OnPlayerMovementStateChanged;

	DECLARE_DELEGATE(FOnSpecialAbilityUsed)
	FOnSpecialAbilityUsed OnSpecialAbilityUsed;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerSwitchedCombatState, bool)
	FOnPlayerSwitchedCombatState OnPlayerSwitchedCombatState;
	
	// Sets default values for this character's properties
	APlayerCharacter( const FObjectInitializer& ObjectInitializer );

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;
	virtual void PostInitializeComponents() override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Sets current movement state
	//Movement state is a class, which responsible
	//For Player's actions
	void SetCurrentState(UPlayerMovementState *NewState, const float& FloatValue = 0, const FVector& VectorValue = FVector::ZeroVector);

	void SetCurrentState(EPlayerMovementStates MovementState, const float& FloatValue = 0, const FVector& VectorValue = FVector::ZeroVector);

	void ResetCurrentState();

	//Current Player State in Enum
	//Used to drive Animations
	UPROPERTY(BlueprintReadWrite)
    TEnumAsByte<PlayerMovementStates::Type> PlayerMovementState = PlayerMovementStates::Run;

	bool bInCombatState = false;
	
	//References to current object, which Player uses to move around
	UPROPERTY()
	APlayerSlider *MovementObject;

	UPROPERTY( EditDefaultsOnly )
	UWeaponryComponent* Weaponry;

	//Movement Manager variable
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UMovementManager *MovementManager;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMomentumAccumulatorComponent* MomentumAccumulator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArmourDamageHandlerComponent *DamageHandlerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerAmmoComponent *AmmoComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerSpeedEnergyComponent *SpeedEnergyComponent;

	UPROPERTY()
	UAdvancedActionsComponent *AdvancedActionsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UKeyHolderComponent *KeyHolderComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Falling() override;
	
	//Landing logic
	virtual void Landed(const FHitResult &Hit) override;

	//Current movement state
	UPROPERTY()
	UPlayerMovementState *CurrentPlayerMovementState;

	//Camera stuff
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent *SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent *Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Control")
	float CameraInterpSpeed = 5.0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Control")
	float SpringArmLengthOffset = 150.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Control")
	float SpringArmDefaultLength = 450.0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Control")
	float SpringArmPositionOffset = 150.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent *WallJumpCheckBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent *MovementTriggerBottom;
	
	void MoveRight(float value);
	void MoveForward(float value);
	void PlayerJumpStart();
	void PlayerJumpEnd();
	void PlayerDash();
	void PlayerSpecialAction();
	void PlayerSlide();
	void PlayerSwitchCombatState();
	void ActivateWeaponSelector();
	void DeactivateWeaponSelector();
	void UseSpecialAbility();
	void PlayerInteract();
	void PlayerCrouchPressed();
	void PlayerCrouchReleased();
	void PlayerCigaretteSmoke();

	//Placeholder
	void Heal();
	void Resupply();

	//Player complete movement stop
	UFUNCTION()
	void PlayerStop(float BrakingFrictionFactorSave,float BrakingFrictionSave);

	void PlayerStop();

	UFUNCTION()
	void ResetPlayerJumps();

	//Construction script
	virtual void OnConstruction(const FTransform &transform) override;

	virtual void Destroyed() override;

	float GetMovementInputRight() const
	{
		return MovementInputRight;
	}

	float GetMovementInputForward() const
	{
		return MovementInputForward;
	}

	float GetSpecialAttackDamage() const
	{
		return SpecialAttackDamage;
	}

	float GetSpecialAttackDistance() const
	{
		return SpecialAttackDistance;
	}

	float GetSpecialAttackSpreadRadius() const
	{
		return SpecialAttackSpreadRadius;
	}

	float GetSpecialAttackDuration() const
	{
		return  SpecialAttackDuration;
	}

	TSubclassOf<UDamageType> GetSpecialAttackDamageType() const
	{
		return SpecialAttackDamageType;
	}

	const APlayerHUD* GetHUD() const {return PlayerHUD;}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Special Attack");
	float SpecialAttackDamage = 100.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Special Attack");
	float SpecialAttackDistance = 500.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Special Attack");
	float SpecialAttackSpreadRadius = 100.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Special Attack");
	float SpecialAttackDuration = 1.0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Special Attack");
	TSubclassOf<UDamageType> SpecialAttackDamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Cigarette Smoke");
	UAnimMontage *CigaretteSmokeMontage;

	void PlayerCrouchAirAttack();

	UPROPERTY()
	FTimerHandle TH_CrouchAttack;

	UPROPERTY()
	bool ShouldCrouchAttack = false;

private:
	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Movement input saves
	float MovementInputRight;
	float MovementInputForward;

	//MovementComponent
	UPROPERTY()
	UPlayerMovementComponent *PlayerMovementComponent;

	UPROPERTY()
	APlayerHUD *PlayerHUD;
};
