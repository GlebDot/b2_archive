#pragma once

#include "CoreMinimal.h"

#include "CharacterMovementParams.h"
#include "Components/ActorComponent.h"
#include "MomentumAccumulatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UMomentumAccumulatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMomentumAccumulatorComponent();

	void Init(const FCharacterMomentumParams& InMomentumParams);

	void AccumulateMomentum(const FVector& CurrentInputVelocity, float DeltaTime);

	void ChangeDirectionPreserveMagnitude(const FVector& NewDirection);
	void ChangeDirectionAndMagnitude(const FVector& NewDirection, float MagnitudeChangePercent);
	void ChangeDirectionWithMaxMagnitude(const FVector& NewDirection);
	void ChangeMagnitudePreserveDirection(float MagnitudeChangePercent);

	FVector GetMomentum() const;
	FVector GetMomentumDirection() const;
	FVector GetMomentumScaled() const;

	float GetMomentumMagnitudeValue() const;
	float GetMomentumMagnitudeRatio() const;

	float GetAdditionalMagnitudeValue() const;
	float GetAdditionalMagnitudeRatioClamped() const;
	float GetAdditionalMagnitudeRatioUnclamped() const;
	
private:
	FVector PrevInputVelocityInput = FVector::ZeroVector;
	FVector Momentum = FVector::ZeroVector;
	FVector MomentumSave = FVector::ZeroVector;
	float MomentumMagnitude = 0.0f;
	float MomentumMagnitudeSave = 0.0f;
	float AdditionalMomentumMagnitudePercent = 0.0f;

	FCharacterMomentumParams Params {};
	
	bool bMomentumCalculationFrozen = false;
	FTimerHandle CalculationFreezeTimerHandle {};
	FVector InputVelocityOnFreeze = FVector::ZeroVector;
	
	void FreezeMomentumCalculation(const FVector& InputVelocity);
	void UnFreezeMomentumCalculation();
	bool ShouldFreezeMagnitudeCalculations(const FVector& CurrentInputVelocity) const;
	void TryToUnFreezeMomentumCalculations(const FVector& InputVelocity);
	
	void ProcessMagnitudeChange(float NewMagnitude);
	
	void CalculateDirectionAndMagnitude(const FVector& CurrentInputVelocity, float DeltaTime);
	void CalculateAdditionalMagnitudePercent(float DeltaTime);

	void ProcessNewInputVelocity(const FVector& CurrentInputVelocity);
	
	void CalculateMomentumInterpSpeeds(float& OutDirectionSpeed, float& OutMagnitudeSpeed, const float VelocityDirectionDelta) const;
};
