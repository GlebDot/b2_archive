// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapons/Base/WeaponryConfig.h"

#include "PlayerWeapons/Base/WeaponBase.h"
#include "Engine/SkeletalMesh.h"
#include "PlayerWeapons/Base/WeaponSocket.h"

bool FWeaponSocketData::IsValid() const
{
	return ::IsValid(WeaponSocket) && ::IsValid(SocketMesh);
}

bool FWeaponSettings::IsValid() const
{
	return ::IsValid(Weapon);
}
