// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/WeaponProjectiles/LauncherProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ALauncherProjectile::ALauncherProjectile()
{
    InitialLifeSpan = 2.5f;
    ProjectileMovement->ProjectileGravityScale = 1.0f;
}

void ALauncherProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->ActorHasTag(FName("Enemy")))
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

        if (OtherActor->IsPendingKill())
        {
            Destroy();
            return;
        }

        ProjectileMovement->StopMovementImmediately();
        AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
    }
}
