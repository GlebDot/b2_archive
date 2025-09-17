// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/UI/PlayerHUD.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/DamageComponents/ArmourDamageHandlerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

APlayerHUD::APlayerHUD()
{
    WeaponSelectorInputMode = FInputModeGameAndUI();
    WeaponSelectorInputMode.SetHideCursorDuringCapture(true);
    InputModeGameOnly = FInputModeGameOnly();
}

void APlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HitNotificationWidgetClass)
    {
        HitNotificationUserWidget = CreateWidget<UPlayerHitNotificationUserWidget>(GetWorld(), HitNotificationWidgetClass);
        if (HitNotificationUserWidget)
        {
            HitNotificationUserWidget->AddToViewport();
        }
    }

    if (PlayerSightWidgetClass)
    {
        PlayerSightUserWidget = CreateWidget<UPlayerSightUserWidget>(GetWorld(), PlayerSightWidgetClass);
        if (PlayerSightUserWidget)
        {
            PlayerSightUserWidget->AddToViewport();
        }
    }

    if (WeaponSelectorWidgetClass)
    {
        WeaponSelectorUserWidget = CreateWidget<UWeaponSelectorUserWidget>(GetWorld(), WeaponSelectorWidgetClass);
        if (WeaponSelectorUserWidget)
        {
            WeaponSelectorUserWidget->AddToViewport();
            WeaponSelectorUserWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    // if (PlayerRef)
    // {
    //     PlayerRef->DamageHandlerComponent->OnPlayerHealthDecreased.BindUFunction(this, FName("OnPlayerTakeDamage"));
    //     // PlayerRef->OnTakeAnyDamage.AddDynamic(this, &APlayerHUD::OnPlayerTakeDamage);
    // }

    PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void APlayerHUD::Reset()
{
    Super::Reset();
    PlayerAmmoChanged(EAmmoRequestType::Heavy, 1.0f);
    PlayerAmmoChanged(EAmmoRequestType::Light, 1.0f);
}

void APlayerHUD::ShowWeaponSelector()
{
    WeaponSelectorUserWidget->SetVisibility(ESlateVisibility::Visible);
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);

    int ViewportX, ViewportY;
    PlayerControllerRef->SetInputMode(WeaponSelectorInputMode);
    PlayerControllerRef->bShowMouseCursor = true;
    PlayerControllerRef->GetViewportSize(ViewportX, ViewportY);
    PlayerControllerRef->SetMouseLocation(ViewportX / 2, ViewportY / 2);
    
    WeaponSelectorUserWidget->WeaponSelectorActivated(PlayerControllerRef);
}

void APlayerHUD::HideWeaponSelector()
{
    if (WeaponSelectorUserWidget)
    {
        WeaponSelectorUserWidget->WeaponSelectorDeactivated();
        WeaponSelectorUserWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    PlayerControllerRef->SetInputMode(InputModeGameOnly);
    PlayerControllerRef->bShowMouseCursor = false;
}

void APlayerHUD::PlayerAmmoChanged(EAmmoRequestType Type, const float Percent) const
{
    switch (Type)
    {
        case EAmmoRequestType::Light:
            PlayerSightUserWidget->SetLeftAmmoPercent(Percent);
            break;
        case EAmmoRequestType::Heavy:
            PlayerSightUserWidget->SetRightAmmoPercent(Percent);
            break;
    }
}

void APlayerHUD::EnemyDamaged()
{
    PlayerSightUserWidget->EnemyDamaged();
}

void APlayerHUD::EnemyDestroyed()
{
    PlayerSightUserWidget->EnemyKilled();
}

void APlayerHUD::SetSightVisibility(ESlateVisibility Visibility)
{
    if (PlayerSightUserWidget)
    {
        PlayerSightUserWidget->SetVisibility(Visibility);
    }
}

void APlayerHUD::Init(APlayerCharacter* Player, UWeaponryComponent* InWeaponryComponent)
{
    PlayerRef = Player;
    WeaponryComponent = InWeaponryComponent;
    if (PlayerRef)
    {
        PlayerRef->DamageHandlerComponent->OnPlayerHealthDecreased.BindUFunction(this, FName("OnPlayerTakeDamage"));
    }
}

// void APlayerHUD::OnPlayerTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
//                                     AController* InstigatedBy, AActor* DamageCauser)
// {
//     if (DamageCauser != PlayerRef && HitNotificationUserWidget)
//     {
//         HitNotificationUserWidget->OnPlayerHit();
//     }
// }

void APlayerHUD::OnPlayerTakeDamage() const
{
    if (HitNotificationUserWidget)
    {
        HitNotificationUserWidget->OnPlayerHit();
    }
}
