// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollisionQueryParams.h"
#include "UObject/Object.h"
#include "ShooterHelper.generated.h"

struct FPreShotInfo;
class UCameraComponent;

USTRUCT()
struct FWeaponDamageInfo
{
	GENERATED_BODY()
	
};

namespace FWeaponShooterHelper
{
	struct FPreShotInfo
	{
		FVector ShotEndLocation = FVector::ZeroVector;
		FVector ShotStartLocation = FVector::ZeroVector;
		TWeakObjectPtr<AActor> PreHitActor {};
		bool IsPreHit = false;;
	};

	// Makes a PreShot, to collect an information about potential targets
	FPreShotInfo PreShotLineCone(
		UCameraComponent* InCameraComponent,
		UWorld* InWorld,
		float TraceDistance,
		const TArray<float>& ConeAngles,
		int32 NumTracesInCone);

	FORCEINLINE FCollisionQueryParams GetWeaponCollisionQueryParams()
	{
		return FCollisionQueryParams::DefaultQueryParam;
	};
	FORCEINLINE FCollisionObjectQueryParams GetWeaponObjectQueryParams()
	{
		FCollisionObjectQueryParams ObjectSettings {};
		ObjectSettings.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1); //Add Enemy trace channel
		ObjectSettings.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectSettings.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		return ObjectSettings;
	}

}
