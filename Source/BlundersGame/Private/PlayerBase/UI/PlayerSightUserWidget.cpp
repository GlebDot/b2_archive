// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/UI/PlayerSightUserWidget.h"

UPlayerSightUserWidget::UPlayerSightUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
}

void UPlayerSightUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    StoreProgressBars();
    
    EnemyHitAnimation = GetAnimationByName(FName("HitNotification"));
    EnemyDeathAnimation = GetAnimationByName(FName("DeathNotification"));

    AmmoLeft_PB = GetProgressBarByName(FName("AmmoLeftPB"));
    AmmoRight_PB = GetProgressBarByName(FName("AmmoRightPB"));

    SetLeftAmmoPercent(1.0);
    SetRightAmmoPercent(1.0);
}

void UPlayerSightUserWidget::EnemyDamaged()
{
    if (EnemyHitAnimation)
    {
        PlayAnimation(EnemyHitAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
    }
}

void UPlayerSightUserWidget::EnemyKilled()
{
    if (EnemyDeathAnimation)
    {
        PlayAnimation(EnemyDeathAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
    }
}

void UPlayerSightUserWidget::SetLeftAmmoPercent(const float Percent) const
{
    AmmoLeft_PB->SetPercent(Percent);
}

void UPlayerSightUserWidget::SetRightAmmoPercent(const float Percent) const
{
    AmmoRight_PB->SetPercent(Percent);
}

UProgressBar* UPlayerSightUserWidget::GetProgressBarByName(const FName ProgressBarName) const
{
    UProgressBar* const* ProgressBar = ProgressBarsMap.Find(ProgressBarName);
    if (ProgressBar)
    {
        return  *ProgressBar;
    }

    return nullptr;
}

void UPlayerSightUserWidget::StoreProgressBars()
{
    ProgressBarsMap.Empty();

    FProperty* Prop = GetClass()->PropertyLink;
    while (Prop)
    {
        if (Prop->GetClass() == FObjectProperty::StaticClass())
        {
            FObjectProperty *ObjProp = CastField<FObjectProperty>(Prop);
            if (ObjProp->PropertyClass == UProgressBar::StaticClass())
            {
                UObject *Obj = ObjProp->GetPropertyValue_InContainer(this);
                UProgressBar *ProgressBar = Cast<UProgressBar>(Obj);
                if (ProgressBar)
                {
                    FName ProgressBarName = FName(ProgressBar->GetName());
                    ProgressBarsMap.Add(ProgressBarName, ProgressBar);
                }
            }
        }

        Prop = Prop->PropertyLinkNext;
    }
}
