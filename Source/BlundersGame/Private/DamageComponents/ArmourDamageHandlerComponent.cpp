// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/ArmourDamageHandlerComponent.h"
#include "GameFramework/Character.h"


UArmourDamageHandlerComponent::UArmourDamageHandlerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    //Armour Mesh
    ArmourDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmourDisplayMesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ArmourMesh(TEXT("StaticMesh'/Game/Player/SuitMeshes/Meshes/PlaceholderArmour.PlaceholderArmour'"));
    if (ArmourMesh.Succeeded())
    {
        ArmourDisplayMesh->SetStaticMesh(ArmourMesh.Object);
    }

    //Health Alert Mesh
    HealthAlertMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthAlertMesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> HealthMesh(TEXT("StaticMesh'/Game/Player/SuitMeshes/Meshes/HealthAlertMesh.HealthAlertMesh'"));
    if (HealthMesh.Succeeded())
    {
        HealthAlertMesh->SetStaticMesh(HealthMesh.Object);
    }
}

void UArmourDamageHandlerComponent::OnComponentCreated()
{
    Super::OnComponentCreated();

    if (ArmourDisplayMesh)
    {
        ACharacter *Character = Cast<ACharacter>(GetOwner());
        ArmourDisplayMesh->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("ArmourDisplaySocket"));
        HealthAlertMesh->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HealthAlertSocket"));
    }
}

float UArmourDamageHandlerComponent::GetArmourPercent()
{
    return CurrentArmour * 1.0 / MaxArmour;
}

void UArmourDamageHandlerComponent::AbsoluteDamage(const int Damage)
{
    CurrentHealth -=Damage;
}

void UArmourDamageHandlerComponent::ReduceHealth(float HealthReducingAmount)
{
    if (HealthReducingAmount > 0.0f && !IsImmortal)
    {
        IsImmortal = true;
        GetWorld()->GetTimerManager().SetTimer(TH_Immortal, this, &UArmourDamageHandlerComponent::ResetImmortal,
                                               ImmortalTime, false);
        
        //Reduce damage by armour amount
        const float ReducedDamage = HealthReducingAmount * (1 - GetArmourPercent());

        //Reduce armour amount
        ReduceArmour(FMath::TruncToInt(HealthReducingAmount));

        //Reduce Health
        CurrentHealth -= FMath::TruncToInt(ReducedDamage);
        CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

        if (OnPlayerHealthDecreased.IsBound())
        {
            OnPlayerHealthDecreased.Execute();
        }
    
        if (GetHealthPercent() < 0.3)
        {
            HealthAlertMesh->SetScalarParameterValueOnMaterials(FName("AlertSwitch"), 1.0f);
        }

        if (CurrentHealth == 0)
        {
            bIsDead = true;
            GetOwner()->Destroy();
        }
    }
}

void UArmourDamageHandlerComponent::ReduceArmour(int ArmourReducing)
{
    CurrentArmour -= ArmourReducing;
    CurrentArmour = FMath::Clamp(CurrentArmour, 0, MaxArmour);
    ArmourDisplayMesh->SetScalarParameterValueOnMaterials(FName("ArmourPercent"), GetArmourPercent());
}

void UArmourDamageHandlerComponent::AddArmourFromPickUp(const int Armour)
{
    CurrentArmour += Armour;
    CurrentArmour = FMath::Clamp(CurrentArmour, 0, MaxArmour);
    ArmourDisplayMesh->SetScalarParameterValueOnMaterials(FName("ArmourPercent"), GetArmourPercent());
}

void UArmourDamageHandlerComponent::AddHealthFromPickUp(const int Health)
{
    CurrentHealth += Health;
    CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
    if (GetHealthPercent() > 0.3f)
    {
        HealthAlertMesh->SetScalarParameterValueOnMaterials(FName("AlertSwitch"), 0.0f);
    }
}

int UArmourDamageHandlerComponent::GetArmour() const
{
    return CurrentArmour;
}


void UArmourDamageHandlerComponent::BeginPlay()
{
    Super::BeginPlay();
    
    CurrentArmour = MaxArmour;
}

void UArmourDamageHandlerComponent::ResetImmortal()
{
    IsImmortal = false;
    GetWorld()->GetTimerManager().ClearTimer(TH_Immortal);
}
