// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerWeapons/Base/WeaponProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//Create Projectile MovementComponent
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->ProjectileGravityScale = 0.0;
	ProjectileMovement->InitialSpeed = 1000.0;
	ProjectileMovement->MaxSpeed = 1000.0;

	//Create Projectile Mesh and Collision 
	ProjectileCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollisionSphere->SetCollisionProfileName(FName("ProjectilePlayer"), true);
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));

	//Set Up Actor structure
	if (ProjectileCollisionSphere)
	{
		SetRootComponent(ProjectileCollisionSphere);
	}
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetupAttachment(ProjectileCollisionSphere);
	InitialLifeSpan = 5.0;

	ProjectileCollisionSphere->AreaClass = nullptr;
}

void AWeaponProjectile::Init(const FWeaponSocketSettingsDamage& InDamageSettings)
{
	DamageSettings = InDamageSettings;
}

void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileCollisionSphere->OnComponentHit.AddDynamic(this, &AWeaponProjectile::OnHit);
}

void AWeaponProjectile::Destroyed()
{
	Super::Destroyed();
	if (ProjectileHitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ProjectileHitEffect, GetActorLocation());
	}

}

//For damage dealing and destroying on overlap
void AWeaponProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (IDamagable* Damagable = Cast<IDamagable>(OtherActor))
		{
			Damagable->ReceiveDamage(DamageSettings.ShotDamageSettings);
		} else
		{
			if (!DamageCauser)
			{
				DamageCauser = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			}

			if (!DamageInstigator)
			{
				DamageInstigator = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			}

			UGameplayStatics::ApplyDamage(
				OtherActor,
				DamageSettings.ShotDamageSettings.BaseDamage,
				DamageInstigator,
				DamageCauser,
				DamageSettings.DamageType);
		}
	}
	Destroy();
}
