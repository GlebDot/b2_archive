// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/DamageHandlerComponent.h"
#include "BlundersGame/Public/DamageComponents/DamageTypes/BlundersDamageType.h"
#include "BlundersGame/Public/Enemies/EnemyBaseCharacter.h"
#include "DamageComponents/DamageTypes.h"
#include "Enemies/EnemyArmour/ArmourContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DamageComponents/DamageTypes/DefaultDamageType.h"
#include "DamageComponents/DamageTypes/SuperEffectDamageType.h"
#include "DamageComponents/StatusEffects/AcidStatusEffect.h"
#include "DamageComponents/StatusEffects/ColdStatusEffect.h"
#include "DamageComponents/StatusEffects/HeatStatusEffect.h"
#include "DamageComponents/StatusEffects/SuperStatusEffect.h"


// Sets default values for this component's properties
UDamageHandlerComponent::UDamageHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDamageHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	EnemyRef = Cast<AEnemyBaseCharacter>(GetOwner());
}

void UDamageHandlerComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{	
	PreviousDamageType = DamageType;

	float ProcessedDamage = Damage;
	if(DamageType->GetClass()->ImplementsInterface(UBlundersDamageType::StaticClass()))
	{
		ProcessedDamage = IBlundersDamageType::Execute_ProcessDamage(DamageType, Damage, DamagedActor);
	}

	if (ProcessedDamage > 0.0f)
	{
		UArmourContainerComponent *ArmourContainerComponent = Cast<UArmourContainerComponent>(GetOwner()->GetComponentByClass(UArmourContainerComponent::StaticClass()));
		if (ArmourContainerComponent)
		{
			ProcessedDamage = ArmourContainerComponent->DealDamageToArmourPlates(Damage, InstigatedBy, DamageCauser, DamageType);
		}

		ReduceHealth(ProcessedDamage);
	}
}

void UDamageHandlerComponent::TrySetNewStatusEffect(const FDamageEffects& Effects)
{
	if (CurrentStatusEffect)
	{
		if (CurrentStatusEffect->GetStatusEffectType() == Effects.StatusEffectType)
		{
			return;
		}

		const EStatusEffectType NewStatusEffectType = CurrentStatusEffect->TrySwitchCurrentState(Effects);

		if (NewStatusEffectType != CurrentStatusEffect->GetStatusEffectType() && NewStatusEffectType != EStatusEffectType::Default)
		{
			SetCurrentStatusEffect(NewStatusEffectType);
		}
	} else
	{
		SetCurrentStatusEffect(Effects.StatusEffectType);
	}
}

void UDamageHandlerComponent::SetCurrentStatusEffect(const EStatusEffectType& NewStatusEffectType)
{
	if (CurrentStatusEffect)
	{
		CurrentStatusEffect->ConditionalBeginDestroy();
	}
	switch (NewStatusEffectType)
	{
	case EStatusEffectType::Heat:
		CurrentStatusEffect = NewObject<UHeatStatusEffect>();
		break;
	case EStatusEffectType::Cold:
		CurrentStatusEffect = NewObject<UColdStatusEffect>();
		break;
	case EStatusEffectType::Super:
		CurrentStatusEffect = NewObject<USuperStatusEffect>();
		break;
	case EStatusEffectType::Acid:
		CurrentStatusEffect = NewObject<UAcidStatusEffect>();
		break;
	default:
		CurrentStatusEffect = nullptr;
		break;
	}

	if (CurrentStatusEffect)
	{
		CurrentStatusEffect->SetEnemyRef(EnemyRef);
		CurrentStatusEffect->OnEffectApplied();
	}
}

EStatusEffectType UDamageHandlerComponent::GetCurrentStatusEffectType() const
{
	if (CurrentStatusEffect)
	{
		return CurrentStatusEffect->GetStatusEffectType();
	}

	return EStatusEffectType::Default;
}

void UDamageHandlerComponent::ReduceHealth(float HealthReducingAmount)
{
	const int HealthReduce = FMath::TruncToInt(HealthReducingAmount);
	TotalDamage += HealthReduce;
	CurrentHealth -= HealthReduce;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	if (EnemyRef)
	{
		EnemyRef->GetMesh()->SetScalarParameterValueOnMaterials(FName("HealthPercent"), 1.0f - GetHealthPercent());
		if (EnemyRef->OnEnemyReceivedDamage.IsBound())
		{
			EnemyRef->OnEnemyReceivedDamage.Execute(HealthReduce);
		}
	}

	if (CurrentHealth == 0)
	{
		bIsDead = true;
		if (CurrentStatusEffect)
		{
			CurrentStatusEffect->OwnerDestroyed();
		}
		
		if (PreviousDamageType)
		{
			if (PreviousDamageType->GetClass()->IsChildOf(USuperEffectDamageType::StaticClass()))
			{
				EnemyRef->SetIsKilledBySuper(true);
			}
		}
		
		GetOwner()->Destroy();
	}
}

void UDamageHandlerComponent::HandleDamage(const FDamageEffects& InputDamage)
{
	//Set current status
	TrySetNewStatusEffect(InputDamage);

	float FinalDamage = InputDamage.BaseDamage;
	if (CurrentStatusEffect)
	{
		//Let the current status effect calculate damage
		FinalDamage = CurrentStatusEffect->ProcessDamage(InputDamage);

		//Update current status effect
		CurrentStatusEffect->UpdateEffect(InputDamage);
	}
	
	//Reduce Health
	ReduceHealth(FinalDamage);
}

void UDamageHandlerComponent::ForceEnemyStun()
{
	if(StunTime > 0.0f)
	{
		// EnemyRef->OnStopEffect();
		OnStopEffectAchieved.Broadcast(StunTime);
	}
}

void UDamageHandlerComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (CurrentStatusEffect)
	{
		CurrentStatusEffect->ConditionalBeginDestroy();
	}
}

