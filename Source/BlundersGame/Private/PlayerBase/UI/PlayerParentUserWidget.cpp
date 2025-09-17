// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/UI/PlayerParentUserWidget.h"

UPlayerParentUserWidget::UPlayerParentUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
}

void UPlayerParentUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    StoreWidgetAnimations();
}

UWidgetAnimation* UPlayerParentUserWidget::GetAnimationByName(const FName AnimationName) const
{
    UWidgetAnimation* const* Animation = AnimationsMap.Find(AnimationName);
    if (Animation)
    {
        return *Animation;
    }

    return nullptr;
}

void UPlayerParentUserWidget::StoreWidgetAnimations()
{
    AnimationsMap.Empty();

    FProperty* Prop = GetClass()->PropertyLink;
    while (Prop)
    {
        if (Prop->GetClass() == FObjectProperty::StaticClass())
        {
            FObjectProperty *ObjProp = CastField<FObjectProperty>(Prop);
            if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
            {
                UObject *Obj = ObjProp->GetPropertyValue_InContainer(this);
                UWidgetAnimation *WidgetAnimation = Cast<UWidgetAnimation>(Obj);
                if (WidgetAnimation && WidgetAnimation->MovieScene)
                {
                    FName AnimName = FName(WidgetAnimation->MovieScene->GetName());
                    AnimationsMap.Add(AnimName, WidgetAnimation);
                }
            }
        }

        Prop = Prop->PropertyLinkNext;
    }
}
