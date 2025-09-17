// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/Explosions/ExplosionWithForce.h"

AExplosionWithForce::AExplosionWithForce()
{
    RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force Component"));
}

void AExplosionWithForce::Destroyed()
{
    Super::Destroyed();
    
    RadialForce->DestroyComponent();
}

void AExplosionWithForce::Explosion()
{
    Super::Explosion();

    RadialForce->FireImpulse();
}
