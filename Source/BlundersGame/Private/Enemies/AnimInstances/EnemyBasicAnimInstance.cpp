// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/AnimInstances/EnemyBasicAnimInstance.h"
#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AActor* UEnemyBasicAnimInstance::GetCurrentEnemyTarget() const
{
    return EnemyRef->GetCurrentTarget();
}

void UEnemyBasicAnimInstance::SetNormalPitch(float Pitch)
{
    NormalPitch = Pitch;
}

float UEnemyBasicAnimInstance::GetNormalPitch() const
{
    return NormalPitch;
}

void UEnemyBasicAnimInstance::SetInterpSpeedCalculation(float InterpSpeed)
{
    InerpSpeedCalculation = InterpSpeed;
}

void UEnemyBasicAnimInstance::SetTargetPitchCalculation(bool IsCalculating)
{
    if (IsCalculating)
    {
        GetWorld()->GetTimerManager().ClearTimer(PitchCalculationTimerHandle);
        CalculatingTargetPitchSwitch(IsCalculating);
    } else
    {
        FTimerDelegate PitchCalculationDelegate;
        PitchCalculationDelegate.BindUFunction(this, FName("CalculatingTargetPitchSwitch"), IsCalculating);
        GetWorld()->GetTimerManager().SetTimer(PitchCalculationTimerHandle, PitchCalculationDelegate, 1.0, false);
    }
}

void UEnemyBasicAnimInstance::CalculateTargetPitch()
{
    AActor *Target = EnemyRef->GetCurrentTarget();
    float LookAtTargetPitch;
    if (Target)
    {
        const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(EnemyRef->GetActorLocation(), Target->GetActorLocation());
        
        LookAtTargetPitch = FMath::FInterpTo(NormalPitch, NewRotation.Pitch, GetWorld()->GetDeltaSeconds(), InerpSpeedCalculation);
    } else
    {
        LookAtTargetPitch = FMath::FInterpTo(NormalPitch, 0, GetWorld()->GetDeltaSeconds(), InerpSpeedCalculation);
    }

    NormalPitch = LookAtTargetPitch;
}

void UEnemyBasicAnimInstance::CalculatingTargetPitchSwitch(bool IsCalculating)
{
    IsCalculatingTargetPitch = IsCalculating;
    if (!IsCalculatingTargetPitch)
    {
        NormalPitch = 0.0;
    }
}

void UEnemyBasicAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    EnemyRef = Cast<AEnemyParent>(TryGetPawnOwner());
}

void UEnemyBasicAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    if (!EnemyRef)
    {
        return;
    }

    Speed = EnemyRef->GetVelocity().Size();
    IsFalling = EnemyRef->GetMovementComponent()->IsFalling();

    if (IsCalculatingTargetPitch)
    {
        CalculateTargetPitch();
    }
}
