// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "ExplosionBasic.generated.h"

UCLASS()
class BLUNDERSGAME_API AExplosionBasic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosionBasic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent *SceneComponent;

	FORCEINLINE void SetDamage(const float Dmg) {Damage = Dmg;}
	FORCEINLINE void SetDamageRadius(const float DmgRadius) {DamageRadius = DmgRadius;}
	FORCEINLINE void SetDamageType(const TSubclassOf<UDamageType> DmgType) {DamageTypeClass = DmgType;}

	FCollisionQueryParams CollisionQueryParams;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Explosion();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Settings")
	float Damage = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Settings")
	float DamageRadius = 300.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Settings")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Settings")
	UNiagaraSystem *ExplosionFX;

	FCollisionShape ExplosionShape;

	FCollisionObjectQueryParams CollisionObjectQueryParams;

	FTimerHandle TH_Explosion;
};
