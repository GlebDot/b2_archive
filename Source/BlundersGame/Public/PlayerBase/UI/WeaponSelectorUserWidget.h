// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "WeaponSelectorUserWidget.generated.h"


UENUM()
enum class EPlayerSelectedActionType : uint8
{
    LightWeapon UMETA(DisplayName="LightWeapon"),
    MiddleWeapon UMETA(DisplayName="MiddleWeapon"),
    HeavyWeapon UMETA(DisplayName="HeavyWeapon"),
    MedKit UMETA(DisplayName="MedKit"),
    AmmoKit UMETA(DisplayName="AmmoKit")
};
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UWeaponSelectorUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UWeaponSelectorUserWidget(const FObjectInitializer &ObjectInitializer);

    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION()
    void WeaponSelectorActivated(APlayerController *Controller);

    UFUNCTION()
    void WeaponSelectorDeactivated();

    UFUNCTION()
    void StoreImages();

    UFUNCTION()
    UImage *GetImageByName(const FName ImageName) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category="SelectorSettings")
    float BaseOpacity = 0.2;

    UPROPERTY(EditDefaultsOnly, Category="SelectorSettings")
    float SelectedOpacity = 0.75;
    
    UPROPERTY()
    UImage *LightWeaponImageRef;

    UPROPERTY()
    UImage *MiddleWeaponImageRef;

    UPROPERTY()
    UImage *HeavyWeaponImageRef;

    UPROPERTY()
    UImage *MedKitImageRef;

    UPROPERTY()
    UImage *AmmoKitImageRef;

    UPROPERTY()
    UImage *CurrentActiveImage;

    UPROPERTY()
    TMap<FName, UImage*> ImagesMap;

    UPROPERTY()
    FVector2D UpVector;

    UPROPERTY()
    EPlayerSelectedActionType CurrentActionType;

    UPROPERTY()
    APlayerCharacter *PlayerRef;

    UPROPERTY()
    APlayerController *ControllerRef;

    UPROPERTY()
    UWeaponryComponent* WeaponryComponent;
};
