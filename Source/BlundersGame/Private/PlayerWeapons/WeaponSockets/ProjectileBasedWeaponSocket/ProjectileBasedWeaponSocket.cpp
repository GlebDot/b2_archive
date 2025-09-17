// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/WeaponSockets/ProjectileBasedWeaponSocket/ProjectileBasedWeaponSocket.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerWeapons/WeaponHelpers/ShooterHelper.h"

void UProjectileBasedWeaponSocket::FireProjectile()
{
    Super::FireProjectile();
    ActivateShotAttributes();
    
    const TArray<float> ConeAngles = {SocketSettingsAimHelper.InnerCircleAngle, SocketSettingsAimHelper.MiddleCircleAngle, SocketSettingsAimHelper.OuterCircleAngle};
    const FWeaponShooterHelper::FPreShotInfo PreShotInfo = FWeaponShooterHelper::PreShotLineCone(
        Player->Camera,
        GetWorld(),
        SocketSettingsAimHelper.TraceDistance,
        ConeAngles,
        SocketSettingsAimHelper.TracesCount);

    if (PreShotInfo.IsPreHit)
    {
        SpawnProjectile(PreShotInfo.ShotEndLocation);
    } else
    {
        //Pre hit failed meaning projectile spawn with no corrections
        SpawnProjectile(PreShotInfo.ShotEndLocation, SocketSettingsBase.SpreadAngle);
    }
}

void UProjectileBasedWeaponSocket::SetupWeaponSettings(UWeaponSocketSettings_Base* InWeaponSettings)
{
    Super::SetupWeaponSettings(InWeaponSettings);

    if (UWeaponSocketSettings_Projectile const* ProjectileSettings = Cast<UWeaponSocketSettings_Projectile>(InWeaponSettings))
    {
        SocketSettingsProjectile = ProjectileSettings->ProjectileSettings;
    }
}

AWeaponProjectile* UProjectileBasedWeaponSocket::SpawnProjectile(const FVector& EndLocation, const float Spread)
{
    const FVector SpawnLocation = SocketMeshComponent->GetSocketLocation(SocketSettingsBase.ShotSocketName);
    FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, EndLocation);

    if (Spread != 0.0)
    {
        const float PitchSpread = FMath::FRandRange(-SocketSettingsBase.SpreadAngle, SocketSettingsBase.SpreadAngle);
        const float YawSpread = FMath::FRandRange(-SocketSettingsBase.SpreadAngle, SocketSettingsBase.SpreadAngle);
        SpawnRotation.Add(PitchSpread, YawSpread, 0.0);
    }

    const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation, FVector(1.0, 1.0, 1.0));
    AWeaponProjectile* Projectile = GetWorld()->SpawnActorDeferred<AWeaponProjectile>(
        SocketSettingsProjectile.WeaponProjectile,
        SpawnTransform,
        GetOwner(),
        GetOwner() ? GetOwner()->GetInstigator() : nullptr);
    Projectile->Init(SocketSettingsDamage);
    Projectile->FinishSpawning(SpawnTransform);
    LastProjectileRef = Projectile;

    return Projectile;
}
