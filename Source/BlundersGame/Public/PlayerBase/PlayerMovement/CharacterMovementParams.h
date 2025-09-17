#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterMovementParams.generated.h"

USTRUCT(BlueprintType)
struct FMovementStateMomentumData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MagnitudeAdditionRatio = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float AdditionalMagnitudeFactor = 0.0f;
};

//TODO: this thing needs to be implemented in movement states
// However I'm not quit sure how to do it properly
// I can always just do it however again
USTRUCT(BlueprintType)
struct FMovementStateData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float StateVelocity = 700.0f;

	UPROPERTY(EditDefaultsOnly)
	float StateEnergyCost = 15.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float StateCooldown = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float MovementArcAngle = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float StateWorkTime = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float MagnitudeChangePercent = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float MagnitudeEffectFactor = 0.0f;

	FMovementStateData& operator*=(float Right);
};

USTRUCT(BlueprintType)
struct FCharacterSpeedData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 900;

	UPROPERTY(EditDefaultsOnly)
	float AdditionalMovementSpeedModifier = 0.25f;
	
};

USTRUCT(BlueprintType)
struct FResourceEnergyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Energy")
	FName MaterialParameterControlName {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Energy")
	float ResourceEnergy = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Energy")
	float ResourceEnergyRecovery = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Energy")
	float ResourceEnergyRecoveryRate = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Energy")
	float ResourceEnergyRecoveryDelay = 1.5;
};

USTRUCT(BlueprintType)
struct FCharacterMomentumParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MaxMomentumMagnitude = 300.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxAdditionalMagnitudePercentStable = 0.25f;

	UPROPERTY(EditDefaultsOnly)
	float MaxAdditionalMagnitudePercentFloating = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float AdditionalMagnitudeCappingSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float AdditionalMagnitudeGainTolerance = 0.05f;

	UPROPERTY(EditDefaultsOnly)
	float AdditionalMagnitudeGainVelocity = 0.05f;

	UPROPERTY(EditDefaultsOnly)
	float MomentumMagnitudeInterpSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly)
	float MomentumDirectionInterpSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float DirectionChangedInterpSpeedMultiplier = 15.0f;

	UPROPERTY(EditDefaultsOnly)
	float DirectionChangedCalculationFreeze = 0.25f;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* MomentumGainCurve {};
};

USTRUCT(BlueprintType)
struct FCharacterMovementSetParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovementSettings|Dash")
	FMovementStateData DashParamsBase {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|Dash")
	FMovementStateData DashParamsVertical {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|Jump")
	FMovementStateData JumpParamsBase {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|Jump")
	FMovementStateData JumpParamsLong {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|Jump")
	FMovementStateData JumpParamsBack {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|Jump")
	FMovementStateData JumpParamsWall {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|DropDown")
	FMovementStateData DropDownParamsBase;
};

UCLASS(BlueprintType)
class BLUNDERSGAME_API UCharacterMovementParams : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovementSettings|Base")
	FName EnergyBagSocketName = "Ribcage";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovementSettings|Base")
	UStaticMesh *BackPackMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSettings|Base")
	int JumpCount = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovementSettings|Base")
	FCharacterSpeedData SpeedParams {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|Energy")
	FResourceEnergyData MovementEnergyParams {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MovementSettings|Energy")
	FResourceEnergyData SupplyEnergyParams {};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovementSettings|MovementSet")
	FCharacterMovementSetParams MovementSetParams {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MovementSettings|Momentum")
	FCharacterMomentumParams MomentumParams {};
};
