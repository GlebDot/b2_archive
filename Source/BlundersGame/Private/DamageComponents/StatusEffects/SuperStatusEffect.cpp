// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/StatusEffects/SuperStatusEffect.h"
#include "BlundersGame/Public/Enemies/EnemyBaseCharacter.h"
#include "BlundersGame/Public/DamageComponents/Explosions/SuperEffectCharge.h"
#include "BlundersGame/Public/DamageComponents/DamageHandlerComponent.h"
#include "DamageComponents/DamageTypes.h"
#include "Enemies/EnemyArmour/ArmourContainerComponent.h"

USuperStatusEffect::USuperStatusEffect()
{
    StatusEffectColor = FVector4(1.0f, 1.0f, 0.0f, 1.0f);
    StatusEffectType = EStatusEffectType::Super;

    static ConstructorHelpers::FObjectFinder<UClass> SuperEffectChargeBP(TEXT("Blueprint'/Game/Enemies/_Base/Blueprints/SupperEffectCharge_BP.SupperEffectCharge_BP_C'"));
    if (SuperEffectChargeBP.Succeeded())
    {
        SuperCharge = SuperEffectChargeBP.Object;
    }
}

void USuperStatusEffect::OnEffectApplied()
{
    IsSuper = true;
    
    if (EnemyRef)
    {
        SuperDamage = EnemyRef->DamageHandler->GetCurrentHealth();
        
        EnemyRef->GetMesh()->SetVectorParameterValueOnMaterials(FName("EffectColorMax"), StatusEffectColor);
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), 1.0f);

        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_SuperEffectWearOff);
        EnemyRef->GetWorld()->GetTimerManager().SetTimer(TH_SuperEffectWearOff, this, &USuperStatusEffect::SuperWearOff, SuperWearOffTime, false);

        UArmourContainerComponent* ArmourContainerComponent = Cast<UArmourContainerComponent>(EnemyRef->GetComponentByClass(UArmourContainerComponent::StaticClass()));
        if (ArmourContainerComponent)
        {
            ArmourContainerComponent->BreakAllArmour();
        }
    }
}

float USuperStatusEffect::ProcessDamage(const FDamageEffects& DamageEffects)
{
    return DamageEffects.BaseDamage;
}

void USuperStatusEffect::OwnerDestroyed()
{
    if (IsSuper && EnemyRef)
    {
        const FVector SpawnLocation = EnemyRef->GetActorLocation();
        ASuperEffectCharge *SuperEffectCharge = EnemyRef->GetWorld()->SpawnActor<ASuperEffectCharge>(SuperCharge, SpawnLocation, FRotator::ZeroRotator);

        //@TODO What super effect damage should be?
        SuperEffectCharge->Damage = SuperDamage;
    }

    SuperWearOff();
}

void USuperStatusEffect::BeginDestroy()
{
    SuperWearOff();
    EnemyRef = nullptr;
    
    Super::BeginDestroy();
}

void USuperStatusEffect::SuperWearOff()
{
    IsSuper = false;

    if (EnemyRef && EnemyRef->GetWorld())
    {
        EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("StatusEffectPercent"), 0.0f);
        EnemyRef->GetWorld()->GetTimerManager().ClearTimer(TH_SuperEffectWearOff);
    }
}
