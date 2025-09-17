// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/Base/WeaponSocket.h"
#include "BlundersGame/Public/PlayerWeapons/Base/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "BlundersGame/Public/PlayerBase/UI/WeaponSocketUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "PlayerWeapons/Base/WeaponryConfig.h"
#include "PlayerWeapons/Base/WeaponSocketTypes_Base.h"
#include "Sound/SoundCue.h"

// Sets default values
UWeaponSocket::UWeaponSocket()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponSocket::SetWeaponState(EWeaponSocketStates NewState)
{
    WeaponState = NewState;
    OnWeaponStateChanged();
}

void UWeaponSocket::PlayAnimationMontages()
{
    UAnimInstance* AnimInstance = (SocketMeshComponent) ? SocketMeshComponent->GetAnimInstance() : nullptr;
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(SocketFireMontage);
    }
    //Playing Player's Animation
    WeaponBase->SocketMontagePlayed();
}

void UWeaponSocket::SetCanShoot(bool bCanShoot)
{
    CanShoot = bCanShoot;
}

bool UWeaponSocket::GetCanShoot() const
{
    return CanShoot;
}

bool UWeaponSocket::ShootCheck() const
{
    return WeaponBase->GetPlayer()->AmmoComponent->AmmoRequestCheck(SocketSettingsAmmo.AmmoType, SocketSettingsAmmo.AmmoPerShot);
}

void UWeaponSocket::ReduceAmmo()
{
    WeaponBase->GetPlayer()->AmmoComponent->ReduceAmmo(SocketSettingsAmmo.AmmoType, SocketSettingsAmmo.AmmoPerShot);
}

void UWeaponSocket::SetInfoVisibility(bool bVisible)
{
    if (SocketWidget)
    {
        const ESlateVisibility Visibility = bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
        SocketWidget->SetVisibility(Visibility);
    }
}

void UWeaponSocket::Init(
    const FWeaponSocketData& InSocketData,
    UPrimitiveComponent* InAttachToComponent,
    UWeaponBase* InWeaponBase,
    APlayerCharacter* InPlayerCharacter)
{
    WeaponBase = InWeaponBase;
    Player = InPlayerCharacter;
    SetupWeaponSettings(InSocketData.WeaponSocketSettings);
    //Set TimeLine and Curve
    if (SocketSettingsFeeling.PitchCurve)
    {
        //Delegate for the recoil function
        FOnTimelineFloat RecoilHandleFunction;

        //Binding recoil function to delegate
        RecoilHandleFunction.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UWeaponSocket, RecoilHandle));

        RecoilTimeline.AddInterpFloat(SocketSettingsFeeling.PitchCurve, RecoilHandleFunction);
        RecoilTimeline.SetLooping(false);
    }

    if (SocketSettingsBase.SocketWidgetClass)
    {
        SocketWidget = CreateWidget<UWeaponSocketUserWidget>(GetWorld(), SocketSettingsBase.SocketWidgetClass);
        if (SocketWidget)
        {
            SocketWidget->AddToViewport();
            SocketWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    CreateVisualComponents(InSocketData, InAttachToComponent);
}

const FDamageEffects& UWeaponSocket::GetDamageSettings()
{
    return SocketSettingsDamage.ShotDamageSettings;
}

void UWeaponSocket::CreateVisualComponents(
    const FWeaponSocketData& InSocketData,
    UPrimitiveComponent* InAttachToComponent)
{
    SocketMeshComponent = NewObject<USkeletalMeshComponent>(Player);
    SocketMeshComponent->SetupAttachment(InAttachToComponent, InSocketData.AttachSocketName);
    SocketMeshComponent->SetSkeletalMesh(InSocketData.SocketMesh);
    SocketMeshComponent->AttachToComponent(
        InAttachToComponent,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        InSocketData.AttachSocketName);
    SocketMeshComponent->RegisterComponent();
}

// Called when the game starts or when spawned
void UWeaponSocket::BeginPlay()
{
    Super::BeginPlay();


}

void UWeaponSocket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (EndPlayReason == EEndPlayReason::Destroyed && IsValid(SocketWidget))
    {
        SocketWidget->RemoveFromParent();
    }
    Super::EndPlay(EndPlayReason);
}

void UWeaponSocket::SetupWeaponSettings(UWeaponSocketSettings_Base* InWeaponSettings)
{
    check(InWeaponSettings);
    SocketSettingsBase = InWeaponSettings->BaseSettings;
    SocketSettingsAmmo = InWeaponSettings->AmmoSettings;
    SocketSettingsAimHelper = InWeaponSettings->AimHelperSettings;
    SocketSettingsDamage = InWeaponSettings->DamageSettings;
    SocketSettingsFeeling = InWeaponSettings->FeelingSettings;
}

void UWeaponSocket::PlayerRecoil()
{
    RecoilTimeline.Stop();
    RecoilTimeline.PlayFromStart();
}

void UWeaponSocket::PlayCameraShake()
{
    APlayerCameraManager *CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    CameraManager->StartCameraShake(SocketSettingsFeeling.WeaponCameraShake);
}

void UWeaponSocket::PlayWeaponSound()
{
    const float Pitch = FMath::RandRange(0.9f, 1.1f);
    UGameplayStatics::PlaySound2D(GetWorld(), SocketSettingsFeeling.WeaponSound, 1, Pitch);
}

float UWeaponSocket::GetShotDelay() const
{
    return 60.0f / SocketSettingsBase.BulletsPerMinute;
}

void UWeaponSocket::ActivateShotAttributes()
{
    PlayAnimationMontages();
    PlayerRecoil();
    PlayCameraShake();
    PlayWeaponSound();
    if (SocketWidget)
    {
        SocketWidget->OnShot();
    }
}

void UWeaponSocket::RecoilHandle(float value)
{
    WeaponBase->GetPlayer()->AddControllerPitchInput(-value * SocketSettingsFeeling.PitchRecoilAmount);
}

void UWeaponSocket::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    RecoilTimeline.TickTimeline(DeltaTime);

    if (SocketSettingsAimHelper.IsDebug)
    {        
        FVector Y = WeaponBase->GetPlayer()->Camera->GetUpVector();
        FVector X = WeaponBase->GetPlayer()->Camera->GetRightVector();
        FVector CircleCenter = WeaponBase->GetPlayer()->Camera->GetComponentLocation();
        CircleCenter += WeaponBase->GetPlayer()->Camera->GetForwardVector() * SocketSettingsAimHelper.TraceDistance;

        //Inner
        float Radius = FMath::Tan(SocketSettingsAimHelper.InnerCircleAngle * PI / 180.0f) * SocketSettingsAimHelper.TraceDistance;
        GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Green, FString::SanitizeFloat(Radius));
        DrawDebugCircle(GetWorld(), CircleCenter, Radius, 40, FColor::Green, false, 0.0, 1, 15, Y, X, false);

        //Middle
        Radius = FMath::Tan(SocketSettingsAimHelper.MiddleCircleAngle * PI / 180.0f) * SocketSettingsAimHelper.TraceDistance;
        GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Orange, FString::SanitizeFloat(Radius));
        DrawDebugCircle(GetWorld(), CircleCenter, Radius, 40, FColor::Orange, false, 0.0, 1, 15, Y, X, false);

        //Outer
        Radius = FMath::Tan(SocketSettingsAimHelper.OuterCircleAngle * PI / 180.0f) * SocketSettingsAimHelper.TraceDistance;
        GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Red, FString::SanitizeFloat(Radius));
        DrawDebugCircle(GetWorld(), CircleCenter, Radius, 40, FColor::Red, false, 0.0, 1, 15, Y, X, false);
    }
}
