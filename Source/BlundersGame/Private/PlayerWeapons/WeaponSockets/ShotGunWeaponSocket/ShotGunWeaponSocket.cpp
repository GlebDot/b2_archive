// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/WeaponSockets/ShotGunWeaponSocket/ShotGunWeaponSocket.h"

#include "Kismet/KismetMathLibrary.h"
#include "PlayerBase/PlayerCharacter.h"
#include "PlayerWeapons/WeaponHelpers/ShooterHelper.h"

void UShotGunWeaponSocket::FireProjectile()
{
    ActivateShotAttributes();
    const TArray<float> ConeAngles = {SocketSettingsAimHelper.InnerCircleAngle, SocketSettingsAimHelper.MiddleCircleAngle, SocketSettingsAimHelper.OuterCircleAngle};
    const FWeaponShooterHelper::FPreShotInfo Info = FWeaponShooterHelper::PreShotLineCone(
        Player->Camera,
        GetWorld(),
        SocketSettingsAimHelper.TraceDistance,
        ConeAngles,
        SocketSettingsAimHelper.TracesCount);
    const FRotator Direction = UKismetMathLibrary::FindLookAtRotation(Info.ShotStartLocation, Info.ShotEndLocation);
    HitScanPellets(Info.ShotStartLocation, Direction.Vector());
}

void UShotGunWeaponSocket::HitScanPellets(const FVector &StartLocation, const FVector& Direction)
{
    const float SpreadRadians = PI / 180.0f * SocketSettingsBase.SpreadAngle;
    FVector EndLocation = FVector::ZeroVector;
    for (int i = 0; i < SocketSettingsBase.BulletsPerShot; i++)
    {
        EndLocation = StartLocation + FMath::VRandCone(Direction, SpreadRadians) * SocketSettingsAimHelper.TraceDistance;

        HitScan(StartLocation, EndLocation);
    } 
}
