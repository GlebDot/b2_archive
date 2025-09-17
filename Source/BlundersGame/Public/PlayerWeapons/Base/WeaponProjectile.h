// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NiagaraSystem.h"
#include "WeaponSocketTypes_Base.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BlundersGame/Public/DamageComponents/Damagable.h"

#include "WeaponProjectile.generated.h"


struct FWeaponSocketSettingsDamage;
class UProjectileMovementComponent;

UCLASS()
class BLUNDERSGAME_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();

	void Init(const FWeaponSocketSettingsDamage& InDamageSettings);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent *ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent *ProjectileCollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
	UNiagaraSystem *ProjectileHitEffect;
protected:
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	AActor *DamageCauser;

	UPROPERTY()
	AController *DamageInstigator;

	FWeaponSocketSettingsDamage DamageSettings {};
};
