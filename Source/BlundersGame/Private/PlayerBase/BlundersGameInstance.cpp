// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/BlundersGameInstance.h"

void UBlundersGameInstance::Init()
{
    Super::Init();

    for (auto &Socket : LightWeaponSockets)
    {
        FPlayerAvailableWeapon AvailableWeaponInfo = FPlayerAvailableWeapon();
        AvailableWeaponInfo.IsBought = true;
        AvailableWeaponInfo.IsEquipped = true;
        AvailableWeaponInfo.SocketType = Socket.Key;
        AvailableWeaponInfo.WeaponSocket = Socket.Value.WeaponSocket;

        AvailableWeapons.Add(AvailableWeaponInfo);
    }

    for (auto &Socket : MiddleWeaponSockets)
    {
        FPlayerAvailableWeapon AvailableWeaponInfo = FPlayerAvailableWeapon();
        AvailableWeaponInfo.IsBought = true;
        AvailableWeaponInfo.IsEquipped = true;
        AvailableWeaponInfo.SocketType = Socket.Key;
        AvailableWeaponInfo.WeaponSocket = Socket.Value.WeaponSocket;

        AvailableWeapons.Add(AvailableWeaponInfo);
    }

    for (auto &Socket : HeavyWeaponSockets)
    {
        FPlayerAvailableWeapon AvailableWeaponInfo = FPlayerAvailableWeapon();
        AvailableWeaponInfo.IsBought = true;
        AvailableWeaponInfo.IsEquipped = true;
        AvailableWeaponInfo.SocketType = Socket.Key;
        AvailableWeaponInfo.WeaponSocket = Socket.Value.WeaponSocket;

        AvailableWeapons.Add(AvailableWeaponInfo);
    }
}
