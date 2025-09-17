#include "PlayerBase/PlayerMovement/MomentumAccumulatorComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UMomentumAccumulatorComponent::UMomentumAccumulatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMomentumAccumulatorComponent::Init(const FCharacterMomentumParams& InMomentumParams)
{
	Params = InMomentumParams;
}

void UMomentumAccumulatorComponent::AccumulateMomentum(const FVector& CurrentInputVelocity, float DeltaTime)
{
	UKismetSystemLibrary::PrintString(
        GetWorld(),
        bMomentumCalculationFrozen ? FString(TEXT("Freeze")) : FString(TEXT("Unfreeze")),
        true,
        false,
        bMomentumCalculationFrozen ? FLinearColor::Blue : FLinearColor::Red, 0.0f);
	TryToUnFreezeMomentumCalculations(CurrentInputVelocity);
	if (!bMomentumCalculationFrozen)
	{
		CalculateDirectionAndMagnitude(CurrentInputVelocity, DeltaTime);
		CalculateAdditionalMagnitudePercent(DeltaTime);
	}
	ProcessNewInputVelocity(CurrentInputVelocity);
}

void UMomentumAccumulatorComponent::ChangeDirectionPreserveMagnitude(const FVector& NewDirection)
{
	Momentum = NewDirection;
}

void UMomentumAccumulatorComponent::ChangeMagnitudePreserveDirection(float MagnitudeChangePercent)
{
	const float NewMagnitude = MomentumMagnitude + Params.MaxMomentumMagnitude * MagnitudeChangePercent;
	ProcessMagnitudeChange(NewMagnitude);
}

void UMomentumAccumulatorComponent::ChangeDirectionAndMagnitude(
	const FVector& NewDirection, float MagnitudeChangePercent)
{
	ChangeMagnitudePreserveDirection(MagnitudeChangePercent);
	ChangeDirectionPreserveMagnitude(NewDirection);
}

void UMomentumAccumulatorComponent::ChangeDirectionWithMaxMagnitude(const FVector& NewDirection)
{
	ChangeDirectionPreserveMagnitude(NewDirection);
	ProcessMagnitudeChange(Params.MaxMomentumMagnitude);
}

FVector UMomentumAccumulatorComponent::GetMomentum() const
{
	return GetMomentumDirection() * MomentumMagnitude;
}

FVector UMomentumAccumulatorComponent::GetMomentumDirection() const
{
	return Momentum.GetSafeNormal2D();
}

FVector UMomentumAccumulatorComponent::GetMomentumScaled() const
{
	return GetMomentumDirection() * MomentumMagnitude / Params.MaxMomentumMagnitude;
}

float UMomentumAccumulatorComponent::GetMomentumMagnitudeValue() const
{
	return MomentumMagnitude;
}

float UMomentumAccumulatorComponent::GetMomentumMagnitudeRatio() const
{
	return MomentumMagnitude / Params.MaxMomentumMagnitude;
}

float UMomentumAccumulatorComponent::GetAdditionalMagnitudeValue() const
{
	return AdditionalMomentumMagnitudePercent;
}

float UMomentumAccumulatorComponent::GetAdditionalMagnitudeRatioClamped() const
{
	return FMath::Clamp(AdditionalMomentumMagnitudePercent / Params.MaxAdditionalMagnitudePercentStable, 0.0f, 1.0f);
}

float UMomentumAccumulatorComponent::GetAdditionalMagnitudeRatioUnclamped() const
{
	return AdditionalMomentumMagnitudePercent / Params.MaxAdditionalMagnitudePercentStable;
}

void UMomentumAccumulatorComponent::FreezeMomentumCalculation(const FVector& InputVelocity)
{
	bMomentumCalculationFrozen = true;
	InputVelocityOnFreeze = PrevInputVelocityInput;
	GetWorld()->GetTimerManager().SetTimer(
        CalculationFreezeTimerHandle,
        this,
        &UMomentumAccumulatorComponent::UnFreezeMomentumCalculation,
        Params.DirectionChangedCalculationFreeze,
        false);
		
}

void UMomentumAccumulatorComponent::UnFreezeMomentumCalculation()
{
	bMomentumCalculationFrozen = false;
	GetWorld()->GetTimerManager().ClearTimer(CalculationFreezeTimerHandle);
}

bool UMomentumAccumulatorComponent::ShouldFreezeMagnitudeCalculations(const FVector& CurrentInputVelocity) const
{
	const bool bAlreadyFreeze = GetWorld()->GetTimerManager().IsTimerActive(CalculationFreezeTimerHandle); 
	if (bAlreadyFreeze || FMath::IsNearlyZero(GetMomentumMagnitudeRatio(), 0.1f) || FMath::IsNearlyZero(PrevInputVelocityInput.SizeSquared2D()))
	{
		return false;
	}

	UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(FMath::Sign(CurrentInputVelocity | PrevInputVelocityInput)), true, false, FLinearColor::Black, 0.0f);

    return FMath::Sign(CurrentInputVelocity | PrevInputVelocityInput) < 0.0f
			|| FMath::IsNearlyZero(CurrentInputVelocity.SizeSquared2D());
}

void UMomentumAccumulatorComponent::TryToUnFreezeMomentumCalculations(const FVector& InputVelocity)
{
	if (!bMomentumCalculationFrozen)
	{
		return;
	}
	
	if (FMath::Sign(InputVelocity | InputVelocityOnFreeze) > 0.0f)
	{
		UnFreezeMomentumCalculation();
	}
}

void UMomentumAccumulatorComponent::ProcessMagnitudeChange(float NewMagnitude)
{
	UnFreezeMomentumCalculation();
	const float MagnitudeDiffPercent = (NewMagnitude - MomentumMagnitude) / Params.MaxMomentumMagnitude;
	MomentumMagnitude = FMath::Clamp(NewMagnitude, 0.0f, Params.MaxMomentumMagnitude);
	AdditionalMomentumMagnitudePercent += MagnitudeDiffPercent * Params.MaxAdditionalMagnitudePercentStable;
	AdditionalMomentumMagnitudePercent = FMath::Clamp(AdditionalMomentumMagnitudePercent, 0.0f, Params.MaxAdditionalMagnitudePercentFloating);
}

void UMomentumAccumulatorComponent::CalculateDirectionAndMagnitude(const FVector& CurrentInputVelocity, float DeltaTime)
{
	MomentumMagnitudeSave = MomentumMagnitude;
	MomentumSave = Momentum;

	const FVector InputVelocityDirection = CurrentInputVelocity.GetSafeNormal2D();
	const FVector MomentumDirection = GetMomentumDirection();
	const float InputVelocityDirectionDelta = FMath::Sign(InputVelocityDirection | MomentumDirection);
	float InterpSpeedMagnitude = 0.0f;
	float InterpSpeedDirection = 0.0f;
	CalculateMomentumInterpSpeeds(InterpSpeedDirection, InterpSpeedMagnitude, InputVelocityDirectionDelta);

	const float InputVelocityMagnitude = InputVelocityDirectionDelta * FMath::Clamp(CurrentInputVelocity.Size2D(), 0.0f, Params.MaxMomentumMagnitude);
	MomentumMagnitude = FMath::FInterpTo(MomentumMagnitude, InputVelocityMagnitude, DeltaTime, InterpSpeedMagnitude);
	MomentumMagnitude = FMath::Clamp(MomentumMagnitude, 0.0f, Params.MaxMomentumMagnitude);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(InputVelocityDirectionDelta), true, false, FLinearColor::Red, 0.0f);
	UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(MomentumMagnitude), true, false, FLinearColor::Yellow, 0.0f);

	if (InputVelocityDirectionDelta > 0.0f || FMath::IsNearlyZero(MomentumMagnitude))
	{
		Momentum = FMath::VInterpTo(
         MomentumDirection,
         CurrentInputVelocity,
         DeltaTime,
         InterpSpeedDirection).GetClampedToMaxSize(1.0f);
	}
}

void UMomentumAccumulatorComponent::CalculateAdditionalMagnitudePercent(float DeltaTime)
{
	if (AdditionalMomentumMagnitudePercent <= Params.MaxAdditionalMagnitudePercentStable)
	{
		const bool bMagnitudeNearlyMax = FMath::IsNearlyEqual(
         MomentumMagnitude,
         Params.MaxMomentumMagnitude,
         Params.AdditionalMagnitudeGainTolerance);

		if (bMagnitudeNearlyMax)
		{
			AdditionalMomentumMagnitudePercent += DeltaTime * Params.AdditionalMagnitudeGainVelocity;
		}
		else
		{
			AdditionalMomentumMagnitudePercent -= DeltaTime * Params.AdditionalMagnitudeGainVelocity * Params.DirectionChangedInterpSpeedMultiplier;
		}
		AdditionalMomentumMagnitudePercent = FMath::Clamp(AdditionalMomentumMagnitudePercent, 0.0f, Params.MaxAdditionalMagnitudePercentStable);
	} else
	{
		AdditionalMomentumMagnitudePercent = FMath::FInterpConstantTo(AdditionalMomentumMagnitudePercent,
			Params.MaxAdditionalMagnitudePercentStable,
			DeltaTime,
			Params.AdditionalMagnitudeCappingSpeed);
	}
	UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(AdditionalMomentumMagnitudePercent), true, false, FLinearColor::Green, 0.0f);
}

void UMomentumAccumulatorComponent::ProcessNewInputVelocity(const FVector& CurrentInputVelocity)
{
	if (ShouldFreezeMagnitudeCalculations(CurrentInputVelocity))
	{
		FreezeMomentumCalculation(CurrentInputVelocity);
	}

	PrevInputVelocityInput = CurrentInputVelocity;
}

void UMomentumAccumulatorComponent::CalculateMomentumInterpSpeeds(
	float& OutDirectionSpeed,
	float& OutMagnitudeSpeed,
	const float VelocityDirectionDelta) const
{
	float InterpSpeedMultiplier = 1.0f;
	if (IsValid(Params.MomentumGainCurve))
	{
		const float CurveTime = GetMomentumMagnitudeRatio();
		InterpSpeedMultiplier = Params.MomentumGainCurve->GetFloatValue(CurveTime);	
	}
	OutMagnitudeSpeed = InterpSpeedMultiplier * Params.MomentumMagnitudeInterpSpeed;
	OutDirectionSpeed = InterpSpeedMultiplier * Params.MomentumDirectionInterpSpeed;

	if ( FMath::IsNearlyEqual(-1.0f, VelocityDirectionDelta) )
	{
		OutMagnitudeSpeed *= Params.DirectionChangedInterpSpeedMultiplier;
		OutDirectionSpeed *= Params.DirectionChangedInterpSpeedMultiplier;
	}
}

