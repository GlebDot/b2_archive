// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/SimpleDamageHandlerComponent.h"

#include "DamageComponents/DamageTypes/BlundersDamageType.h"

// Sets default values for this component's properties
USimpleDamageHandlerComponent::USimpleDamageHandlerComponent()
{

}

void USimpleDamageHandlerComponent::SetMaxHealth(const float Health)
{
	MaxHealth = Health;
	CurrentHealth = MaxHealth;
}


void USimpleDamageHandlerComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{	
	float ProcessedDamage = Damage;
	
	if(DamageType->GetClass()->ImplementsInterface(UBlundersDamageType::StaticClass()))
	{
		ProcessedDamage = IBlundersDamageType::Execute_ProcessDamage(DamageType, Damage, DamagedActor);
	}
	
	ReduceHealth(ProcessedDamage);
}

void USimpleDamageHandlerComponent::ReduceHealth(float HealthReducingAmount)
{
	CurrentHealth -= FMath::TruncToInt(HealthReducingAmount);
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	if (CurrentHealth == 0)
	{
		bIsDead = true;
		GetOwner()->Destroy();
	}
}

bool USimpleDamageHandlerComponent::IsDead() const
{
	return bIsDead;
}

float USimpleDamageHandlerComponent::GetHealthPercent() const
{
	return CurrentHealth * 1.0 / MaxHealth;
}

void USimpleDamageHandlerComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	CurrentHealth = MaxHealth;
	AActor *Owner = GetOwner();
	if (Owner)
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USimpleDamageHandlerComponent::TakeDamage);
	}	
}

