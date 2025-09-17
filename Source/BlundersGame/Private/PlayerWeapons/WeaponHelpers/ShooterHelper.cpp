// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapons/WeaponHelpers/ShooterHelper.h"

#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "PlayerWeapons/Base/WeaponSocket.h"

namespace FWeaponShooterHelperLocal
{
	struct FPreShotConeTraceInfo
	{
		float ConeAngle = 0.0f;
		int16 TracesNum = 0;
		float TraceDistance = 0.0f;
		TWeakObjectPtr<UCameraComponent> Camera {};
		TWeakObjectPtr<UWorld> World {};
		FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
		FCollisionObjectQueryParams ObjectQueryParams {};

		bool IsValid() const
		{
			return Camera.IsValid() && World.IsValid() && ConeAngle > 0.0f && TracesNum > 0 && TraceDistance > 0.0f;
		};
	};

	struct FPreShotLineTraceInfo
	{
		float TraceDistance = 0.0f;
		TWeakObjectPtr<UCameraComponent> Camera {};
		TWeakObjectPtr<UWorld> World {};
		FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
		FCollisionObjectQueryParams ObjectQueryParams {};

		bool IsValid() const
		{
			return Camera.IsValid() && World.IsValid() && TraceDistance > 0.0f;
		};
	};


	bool PreShotLineTraceSingle(
		const FPreShotLineTraceInfo& InLineTraceInfo,
		FWeaponShooterHelper::FPreShotInfo& PreShot)
	{
		check(InLineTraceInfo.IsValid());

		UCameraComponent const* Camera = InLineTraceInfo.Camera.Get();
		UWorld const* World = InLineTraceInfo.World.Get(); 
	
		//Configure Trace start location and Trace end location
		const FVector TraceStart = Camera->GetComponentLocation();
		const FVector TraceEnd = Camera->GetComponentRotation().Vector() * InLineTraceInfo.TraceDistance + TraceStart;
		PreShot.ShotStartLocation = TraceStart;

		FHitResult HitInfo = FHitResult(ForceInit);

		const bool bIsHit = World->LineTraceSingleByObjectType(
			HitInfo, //Hit information
			TraceStart, //Start
			TraceEnd, //End
			InLineTraceInfo.ObjectQueryParams, //Settings for objects to trace against
			InLineTraceInfo.CollisionQueryParams); //Settings for tracing

		if (bIsHit && HitInfo.GetActor() != nullptr)
		{
			PreShot.ShotEndLocation = HitInfo.Location;

			if (HitInfo.Actor->ActorHasTag(FName(TEXT("Enemy"))))
			{
				PreShot.PreHitActor = HitInfo.GetActor();
				PreShot.IsPreHit = true;
				return true;
			}
		}
		else
		{
			PreShot.ShotEndLocation = TraceEnd;
		}

		return false;
	}

	bool PreShotLineTraceCone(
		const FPreShotConeTraceInfo& InConeTraceInfo,
		FWeaponShooterHelper::FPreShotInfo& PreShot)
	{
		check(InConeTraceInfo.IsValid());
	
		const UCameraComponent* Camera = InConeTraceInfo.Camera.Get();
		const UWorld* World = InConeTraceInfo.World.Get();
	
		//Get Trace Start
		const FVector TraceStart = Camera->GetComponentLocation();

		//Get the initial direction (forward vector of camera)
		FVector TraceDirection = Camera->GetComponentRotation().Vector();

		PreShot.ShotStartLocation = TraceStart;
		PreShot.ShotEndLocation = TraceStart + TraceDirection * InConeTraceInfo.TraceDistance;

		//Get vector to rotate initial direction
		FVector VectorRotation = Camera->GetRightVector();

		//Rotate initial direction by ConeAngle (a)
		/*     ||
		 *    |a|
		 *   |  |
		 *  |   |
		 * |    |
		 *|     |
		 */
		TraceDirection = TraceDirection.RotateAngleAxis(InConeTraceInfo.ConeAngle, VectorRotation);

		//Set new rotation vector in order to form an actual cone 
		VectorRotation = Camera->GetForwardVector();

		//How angle much rotation should be applied every iteration
		const float RotationStep = 360 / InConeTraceInfo.TracesNum;

		//Storage for hit result
		FHitResult HitInfo = FHitResult(ForceInit);

		for (int i = 0; i < InConeTraceInfo.TracesNum; i++)
		{
			//Calculating trace end
			FVector TraceEnd = TraceDirection * InConeTraceInfo.TraceDistance + TraceStart;

			//Tracing
			const bool bIsHit = World->LineTraceSingleByObjectType(
				HitInfo, //Hit information
				TraceStart, //Start
				TraceEnd, //End
				InConeTraceInfo.ObjectQueryParams, //Settings for objects to trace against
				InConeTraceInfo.CollisionQueryParams); //Settings for tracing

			if (bIsHit && HitInfo.GetActor())
			{
				if (HitInfo.Actor->ActorHasTag(FName(TEXT("Enemy"))))
				{
					PreShot.ShotEndLocation = HitInfo.Location;
					PreShot.IsPreHit = true;
					PreShot.PreHitActor = HitInfo.GetActor();
					return true;
				}
			}
		
			//Each iteration changing TraceDirection in order to get a cone trace
			TraceDirection = TraceDirection.RotateAngleAxis(RotationStep, VectorRotation);
		}

		return false;
	}
}

FWeaponShooterHelper::FPreShotInfo FWeaponShooterHelper::PreShotLineCone(
	UCameraComponent* InCameraComponent,
	UWorld* InWorld,
	float TraceDistance,
	TArray<float> const& ConeAngles,
	int32 NumTracesInCone)
{
	FPreShotInfo PreShotInfo = FPreShotInfo();

	FWeaponShooterHelperLocal::FPreShotLineTraceInfo LineTraceInfo {};
	LineTraceInfo.Camera = InCameraComponent;
	LineTraceInfo.World = InWorld;
	LineTraceInfo.CollisionQueryParams = GetWeaponCollisionQueryParams();
	LineTraceInfo.ObjectQueryParams = GetWeaponObjectQueryParams();
	LineTraceInfo.TraceDistance = TraceDistance;

	if (FWeaponShooterHelperLocal::PreShotLineTraceSingle(LineTraceInfo, PreShotInfo))
	{
		return PreShotInfo;
	}

	FWeaponShooterHelperLocal::FPreShotConeTraceInfo ConeTraceInfo {};
	ConeTraceInfo.World = InWorld;
	ConeTraceInfo.Camera = InCameraComponent;
	ConeTraceInfo.CollisionQueryParams = GetWeaponCollisionQueryParams();
	ConeTraceInfo.ObjectQueryParams = GetWeaponObjectQueryParams();
	ConeTraceInfo.TraceDistance = TraceDistance;
	ConeTraceInfo.TracesNum = NumTracesInCone;

	for (float ConeAngle : ConeAngles)
	{
		ConeTraceInfo.ConeAngle = ConeAngle;
		if (FWeaponShooterHelperLocal::PreShotLineTraceCone(ConeTraceInfo, PreShotInfo))
		{
			return PreShotInfo;
		}
	}

	return PreShotInfo;
}
