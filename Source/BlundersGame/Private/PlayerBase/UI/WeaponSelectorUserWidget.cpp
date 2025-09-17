// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/UI/WeaponSelectorUserWidget.h"
#include "BlundersGame/Public/PlayerBase/Components/PlayerSpeedEnergyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerWeapons/Base/WeaponryComponent.h"

UWeaponSelectorUserWidget::UWeaponSelectorUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    UpVector = FVector2D(0.0f, 1.0f);
}

void UWeaponSelectorUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    StoreImages();

    LightWeaponImageRef = GetImageByName(FName("LightWeaponImage"));
    MiddleWeaponImageRef = GetImageByName(FName("MiddleWeaponImage"));
    HeavyWeaponImageRef = GetImageByName(FName("HeavyWeaponImage"));
    MedKitImageRef = GetImageByName(FName("MedicalKitImage"));
    AmmoKitImageRef = GetImageByName(FName("AmmoKitImage"));

    CurrentActiveImage = GetImageByName(FName("LightWeaponImage"));;
    CurrentActionType = EPlayerSelectedActionType::LightWeapon;
    PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UWeaponSelectorUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FVector2D CurrentMousePosition;
    FIntVector ViewportSize;
    ControllerRef->GetViewportSize(ViewportSize.X, ViewportSize.Y);
    ControllerRef->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
    CurrentMousePosition.X -= ViewportSize.X / 2;
    CurrentMousePosition.Y -= ViewportSize.Y / 2;
    
    float AngleMultiplier = 1.0;
    if (CurrentMousePosition.X < 0)
    {
        AngleMultiplier = -1.0f;
    }
    CurrentMousePosition.Normalize();
    
    float DeltaRotation  = 180.0f * FMath::Acos(CurrentMousePosition | UpVector) / PI * AngleMultiplier;
    if (DeltaRotation < 0.0f)
    {
        DeltaRotation = 360 + DeltaRotation;
    }


    CurrentActiveImage->SetOpacity(BaseOpacity);
    if (135< DeltaRotation && DeltaRotation < 225)
    {
        CurrentActiveImage = LightWeaponImageRef;
        CurrentActionType = EPlayerSelectedActionType::LightWeapon;
    } 
    if (45 < DeltaRotation && DeltaRotation< 135)
    {
        CurrentActiveImage = MiddleWeaponImageRef;
        CurrentActionType = EPlayerSelectedActionType::MiddleWeapon;
    }
    if (225< DeltaRotation && DeltaRotation < 315)
    {
        CurrentActiveImage = HeavyWeaponImageRef;
        CurrentActionType = EPlayerSelectedActionType::HeavyWeapon;
    }
    if (315 < DeltaRotation && DeltaRotation < 360)
    {
        CurrentActiveImage = MedKitImageRef;
        CurrentActionType = EPlayerSelectedActionType::MedKit;
    }
    if (0 < DeltaRotation && DeltaRotation < 45)
    {
        CurrentActiveImage = AmmoKitImageRef;
        CurrentActionType = EPlayerSelectedActionType::AmmoKit;
    }
    CurrentActiveImage->SetOpacity(SelectedOpacity);
}

void UWeaponSelectorUserWidget::WeaponSelectorActivated(APlayerController* Controller)
{
    if (!ControllerRef)
    {
        ControllerRef = Controller;
    }
}

void UWeaponSelectorUserWidget::WeaponSelectorDeactivated()
{
    if (!IsValid(PlayerRef) || !IsValid(WeaponryComponent))
    {
        PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        WeaponryComponent = PlayerRef->Weaponry;
    }
    
    switch (CurrentActionType)
    {
        case EPlayerSelectedActionType::LightWeapon:
            // WeaponryComponent->WeaponSwitch(FKey("1"));
            break;
        case EPlayerSelectedActionType::MiddleWeapon:
            // PlayerRef->PlayerSwitchWeaponMiddle();
            break;
        case EPlayerSelectedActionType::HeavyWeapon:
            //PlayerRef->PlayerSwitchWeaponHeavy();
            break;
        case EPlayerSelectedActionType::MedKit:
            PlayerRef->SpeedEnergyComponent->UseEnergy(ESpeedEnergyUsageType::Health);
            break;
        case EPlayerSelectedActionType::AmmoKit:
            PlayerRef->SpeedEnergyComponent->UseEnergy(ESpeedEnergyUsageType::Ammo);
            break;
    }
}

void UWeaponSelectorUserWidget::StoreImages()
{
    ImagesMap.Empty();
    
    FProperty* Prop = GetClass()->PropertyLink;
    while (Prop)
    {
        if (Prop->GetClass() == FObjectProperty::StaticClass())
        {
            FObjectProperty *ObjProp = CastField<FObjectProperty>(Prop);
            if (ObjProp->PropertyClass == UImage::StaticClass())
            {
                UObject *Obj = ObjProp->GetPropertyValue_InContainer(this);
                UImage *Image = Cast<UImage>(Obj);
                if (Image)
                {
                    Image->SetOpacity(BaseOpacity);
                    FName ProgressBarName = FName(Image->GetName());
                    ImagesMap.Add(ProgressBarName, Image);
                }
            }
        }

        Prop = Prop->PropertyLinkNext;
    }
}

UImage* UWeaponSelectorUserWidget::GetImageByName(const FName ImageName) const
{
    UImage* const* Image = ImagesMap.Find(ImageName);
    if (Image)
    {
        return *Image;
    }

    return nullptr;
}
