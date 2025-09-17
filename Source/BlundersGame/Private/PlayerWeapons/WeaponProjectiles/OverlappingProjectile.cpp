// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/WeaponProjectiles/OverlappingProjectile.h"
#include "Kismet/GameplayStatics.h"

AOverlappingProjectile::AOverlappingProjectile()
{
    CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
    CapsuleCollision->SetCollisionProfileName(FName("ProjectilePlayer"), true);
    CapsuleCollision->SetupAttachment(ProjectileCollisionSphere);
}

void AOverlappingProjectile::OnCapsuleOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult & SweepResult)
{
    if (IDamagable* Damagable = Cast<IDamagable>(OtherActor))
    {
        Damagable->ReceiveDamage(DamageSettings.ShotDamageSettings);
    } else
    {
        UGameplayStatics::ApplyDamage(
            OtherActor,
            DamageSettings.ShotDamageSettings.BaseDamage,
            DamageInstigator,
            DamageCauser,
            DamageSettings.DamageType);
    }
}

void AOverlappingProjectile::BeginPlay()
{
    Super::BeginPlay();
    CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &AOverlappingProjectile::OnCapsuleOverlap);
}
