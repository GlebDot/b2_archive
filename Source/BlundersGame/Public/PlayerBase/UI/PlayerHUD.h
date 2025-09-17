// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerHitNotificationUserWidget.h"
#include "PlayerSightUserWidget.h"
#include "WeaponSelectorUserWidget.h"
#include "GameFramework/HUD.h"
#include "BlundersGame/Public/PlayerBase/Components/PlayerAmmoComponent.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
    APlayerHUD();
    
    virtual void BeginPlay() override;

    virtual void Reset() override;

    UFUNCTION()
    void ShowWeaponSelector();

    UFUNCTION()
    void HideWeaponSelector();

    UFUNCTION()
    void PlayerAmmoChanged(EAmmoRequestType Type, const float Percent) const;

    UFUNCTION()
    void EnemyDamaged();

    UFUNCTION()
    void EnemyDestroyed();

    UFUNCTION()
    void SetSightVisibility(ESlateVisibility Visibility);

    UFUNCTION()
    void Init(APlayerCharacter* Player, UWeaponryComponent* InWeaponryComponent);

    UPROPERTY(EditDefaultsOnly, Category="PlayerInfo")
    TSubclassOf<UUserWidget> HitNotificationWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category="PlayerInfo")
    TSubclassOf<UUserWidget> PlayerSightWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category="PlayerInfo")
    TSubclassOf<UUserWidget> WeaponSelectorWidgetClass;

protected:
    // UFUNCTION()
    // void OnPlayerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void OnPlayerTakeDamage() const;
    
private:
    UPROPERTY()
    UPlayerHitNotificationUserWidget *HitNotificationUserWidget;

    UPROPERTY()
    UPlayerSightUserWidget *PlayerSightUserWidget;

    UPROPERTY()
    UWeaponSelectorUserWidget *WeaponSelectorUserWidget;

    UPROPERTY()
    class APlayerCharacter *PlayerRef;

    UPROPERTY()
    APlayerController *PlayerControllerRef;

    FInputModeGameAndUI WeaponSelectorInputMode;

    FInputModeGameOnly InputModeGameOnly;

    UPROPERTY()
    UWeaponryComponent* WeaponryComponent;
};
