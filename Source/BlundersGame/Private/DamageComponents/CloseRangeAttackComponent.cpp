// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/DamageComponents/CloseRangeAttackComponent.h"

#include "DamageComponents/DamageHandlerComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemies/EnemyParent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCloseRangeAttackComponent::UCloseRangeAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Create Collision Sphere
	AttackDetectorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Detector Sphere"));
	AttackDetectorSphere->SetCollisionProfileName(FName("Trigger"), true);
	AttackDetectorSphere->SetCanEverAffectNavigation(false);

	DamagingActor = nullptr;
}

void UCloseRangeAttackComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	AActor *Owner = GetOwner();
	if(Owner)
	{
		AttackDetectorSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	AttackDetectorSphere->SetSphereRadius(Radius);
}

void UCloseRangeAttackComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Deactivate();
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}


// Called when the game starts
void UCloseRangeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	Activate();
	DamageCauser = GetOwner();

	if (DamageCauser)
	{
		DamageInstigator = GetOwner()->GetInstigatorController();
	} else
	{
		DamageInstigator = nullptr;
	}

	AEnemyParent *Enemy = Cast<AEnemyParent>(GetOwner());
	if (Enemy)
	{
		Enemy->DamageHandler->OnStopEffectAchieved.AddUFunction(this, FName("OnEnemyStunned"));
	}
}

void UCloseRangeAttackComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	
	//Binding functions to overlap events
	AttackDetectorSphere->OnComponentBeginOverlap.AddDynamic(this, &UCloseRangeAttackComponent::ComponentBeginOverlap);
	AttackDetectorSphere->OnComponentEndOverlap.AddDynamic(this, &UCloseRangeAttackComponent::ComponentEndOverlap);
	AttackDetectorSphere->Activate();
}

void UCloseRangeAttackComponent::Deactivate()
{
	if (GetWorld())
	{	
		GetWorld()->GetTimerManager().ClearTimer(TH_Damage);
	}
	AttackDetectorSphere->OnComponentBeginOverlap.Clear();
	AttackDetectorSphere->OnComponentEndOverlap.Clear();
	AttackDetectorSphere->Deactivate();

	Super::Deactivate();
}

void UCloseRangeAttackComponent::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                       bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ActorToDamage) && DamagingActor == nullptr)
	{
		const float DamageTick = 60.0f / AttacksPerMinute;
		const float DamageTickDelay = FMath::Clamp(Radius / OtherActor->GetVelocity().Size(), 0.0f, DamageTick);
		DamagingActor = OtherActor;
		GetWorld()->GetTimerManager().SetTimer(TH_Damage, this, &UCloseRangeAttackComponent::DealDamage, DamageTick, true, DamageTickDelay);
	}
}

void UCloseRangeAttackComponent::ComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == DamagingActor)
	{
		DamagingActor = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(TH_Damage);
	}
}

void UCloseRangeAttackComponent::DealDamage()
{
	if (DamagingActor)
	{
		UGameplayStatics::ApplyDamage(DamagingActor, Damage, DamageInstigator, DamageCauser, DamageType);
		if (HitEffect)
		{
			UNiagaraComponent *Component = UNiagaraFunctionLibrary::SpawnSystemAttached(HitEffect,
																						AttackDetectorSphere,
																						NAME_None,
																						GetOwner()->GetActorLocation(),
																						FRotator::ZeroRotator,
																						EAttachLocation::KeepWorldPosition,
																						true);
			if (Component && DamagingActor)
			{
				Component->SetNiagaraVariableVec3(FString("Beam End"), DamagingActor->GetActorLocation());
			}
		}
	}
}

void UCloseRangeAttackComponent::OnEnemyStunned(float StunTime)
{
	if (IsActive())
	{
		this->Deactivate();
		GetWorld()->GetTimerManager().SetTimer(TH_Stun, this, &UCloseRangeAttackComponent::OnEnemyStunnedOut, StunTime, false);
	}
}

void UCloseRangeAttackComponent::OnEnemyStunnedOut()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_Stun);
	Activate();
}