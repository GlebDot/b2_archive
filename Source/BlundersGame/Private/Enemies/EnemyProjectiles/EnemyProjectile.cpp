// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/EnemyProjectiles/EnemyProjectile.h"


#include "Enemies/EnemyProjectiles/EnemyProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UEnemyProjectileMovementComponent>(TEXT("Projectile Movement"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->ProjectileGravityScale = 0.0;
		ProjectileMovementComponent->InitialSpeed = 1000.0;
		ProjectileMovementComponent->MaxSpeed = 1000.0;
	}

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	if (SphereCollision)
	{
		SetRootComponent(SphereCollision);
		SphereCollision->SetCollisionProfileName(FName("ProjectileEnemy"), true);
	}

	InitialLifeSpan = 4.0;
}

void AEnemyProjectile::OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetInstigatorController(), this, DamageType);
	Destroy();
}

void AEnemyProjectile::SetTarget(AActor *Target)
{
	ProjectileTarget = Target;
	if (IsHoming)
	{
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingAccelerationMagnitude = HomingForce;
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
	}
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &AEnemyProjectile::OnHitActor);
}

void AEnemyProjectile::Reset()
{
	Super::Reset();
	Destroy();
}

