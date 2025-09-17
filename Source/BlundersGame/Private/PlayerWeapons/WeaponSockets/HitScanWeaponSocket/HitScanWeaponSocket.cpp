// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/WeaponSockets/HitScanWeaponSocket/HitScanWeaponSocket.h"
#include "BlundersGame/Public/PlayerWeapons/Base/WeaponBase.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerWeapons/WeaponHelpers/ShooterHelper.h"

void UHitScanWeaponSocket::FireProjectile()
{
    Super::FireProjectile();
    ActivateShotAttributes();

    const TArray<float> ConeAngles = {SocketSettingsAimHelper.InnerCircleAngle, SocketSettingsAimHelper.MiddleCircleAngle, SocketSettingsAimHelper.OuterCircleAngle};
    const FWeaponShooterHelper::FPreShotInfo Info = FWeaponShooterHelper::PreShotLineCone(
        Player->Camera,
        GetWorld(),
        SocketSettingsAimHelper.TraceDistance,
        ConeAngles,
        SocketSettingsAimHelper.TracesCount);
    
    if (Info.IsPreHit && Info.PreHitActor.IsValid())
    {
        AActor* PreHitActor = Info.PreHitActor.Get();
        if (IDamagable* Damagable = Cast<IDamagable>(PreHitActor))
        {
            Damagable->ReceiveDamage(SocketSettingsDamage.ShotDamageSettings);
        } else
        {
            SimpleDamage(PreHitActor);
        }
        SpawnNiagaraSystem(Info.ShotEndLocation);
    }
    else
    {
        FRotator ScanRotation = UKismetMathLibrary::FindLookAtRotation(Info.ShotStartLocation, Info.ShotEndLocation);
        const float PitchOffset = FMath::FRandRange(-SocketSettingsBase.SpreadAngle, SocketSettingsBase.SpreadAngle);
        const float YawOffset = FMath::FRandRange(-SocketSettingsBase.SpreadAngle, SocketSettingsBase.SpreadAngle);
        ScanRotation.Add(PitchOffset, YawOffset, 0.0);
        const FVector ScanEndLocation = Info.ShotStartLocation + ScanRotation.Vector() * SocketSettingsAimHelper.TraceDistance;
        
        HitScan(Info.ShotStartLocation, ScanEndLocation);
    }
}

void UHitScanWeaponSocket::SetupWeaponSettings(UWeaponSocketSettings_Base* InWeaponSettings)
{
    Super::SetupWeaponSettings(InWeaponSettings);

    if (UWeaponSocketSettings_HitScan const* HitScanSettings = Cast<UWeaponSocketSettings_HitScan>(InWeaponSettings))
    {
        SocketSettingsVisual = HitScanSettings->VisualSettings;
    }
}

void UHitScanWeaponSocket::HitScan(const FVector& StartLocation, const FVector& EndLocation)
{
    FHitResult HitResult = FHitResult(ForceInit);

    const bool IsHit = GetWorld()->LineTraceSingleByObjectType(
        HitResult,
        StartLocation,
        EndLocation,
        FWeaponShooterHelper::GetWeaponObjectQueryParams(),
        FWeaponShooterHelper::GetWeaponCollisionQueryParams());

    FVector BeamEndLocation;
    if (IsHit && HitResult.GetActor())
    {
        BeamEndLocation = HitResult.Location;
        if (IDamagable* Damagable = Cast<IDamagable>(HitResult.GetActor()))
        {
            Damagable->ReceiveDamage(SocketSettingsDamage.ShotDamageSettings);
        } else
        {
            SimpleDamage(HitResult.GetActor());
        }
    } else
    {
        BeamEndLocation = EndLocation;
    }

    SpawnNiagaraSystem(BeamEndLocation);
}

void UHitScanWeaponSocket::SimpleDamage(AActor* ActorToDamage)
{
    if (!DamageCauser)
    {
        DamageCauser = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    }

    if (!DamageInstigator)
    {
        DamageInstigator = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    }
    UGameplayStatics::ApplyDamage(
        ActorToDamage,
        SocketSettingsDamage.ShotDamageSettings.BaseDamage,
        DamageInstigator,
        DamageCauser,
        SocketSettingsDamage.DamageType);
}

void UHitScanWeaponSocket::SpawnNiagaraSystem(const FVector& BeamEnd) const
{
    const FVector SpawnLocation = SocketMeshComponent->GetSocketLocation(SocketSettingsBase.ShotSocketName);
    UNiagaraComponent* NiagaraSystemRef = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        SocketSettingsVisual.TrailFX,
        SpawnLocation);
    NiagaraSystemRef->SetNiagaraVariableVec3(TEXT("Beam End"), BeamEnd);
}
