// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/EnemyBaseCharacter.h"
#include "DamageComponents/DamageHandlerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BlundersGame/Public/Enemies/EnemyArmour/ArmourContainerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase/UI/PlayerHUD.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/Enemies/Misc/EnemyBaseInfoDataAsset.h"

// Sets default values
AEnemyBaseCharacter::AEnemyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageHandler = CreateDefaultSubobject<UDamageHandlerComponent>(TEXT("Damage Handler"));
	GetMesh()->SetCollisionProfileName(FName("EnemyCharacter"), true);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(FName("Enemy"));

	bUseControllerRotationYaw = false;
}

void AEnemyBaseCharacter::ReceiveDamage(const FDamageEffects& DamageEffects)
{
	DamageHandler->HandleDamage(DamageEffects);

	if (!PlayerHUD)
	{
		PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	}

	if (PlayerHUD)
	{
		PlayerHUD->EnemyDamaged();
	}
}

void AEnemyBaseCharacter::OnActorDestroyed(AActor* DestroyedActor)
{
	if (DamageHandler->IsDead())
	{
		DamageHandler->OnStopEffectAchieved.Clear();
		if (DamageHandler->GetCurrentStatusEffectType() != EStatusEffectType::Super && !IsKilledBySuper)
		{
			FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			APickUpSpawner *Spawner = GetWorld()->SpawnActor<APickUpSpawner>(APickUpSpawner::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
			switch (PickUpAmount)
			{
			case EPickUpAmount::Large:
				Spawner->SetPickUpsAmountToSpawn(PICKUP_SIZE_LARGE);
				break;
			case EPickUpAmount::Middle:
				Spawner->SetPickUpsAmountToSpawn(PICKUP_SIZE_MIDDLE);
				break;
			case EPickUpAmount::Small:
				Spawner->SetPickUpsAmountToSpawn(PICKUP_SIZE_SMALL);
				break;		
			case EPickUpAmount::Zero:
				Death();
				return;
			}
			Spawner->SpawnPickUps(PickUpType);
		}

		Death();
	}
}

void AEnemyBaseCharacter::Death_Implementation()
{
	if (!PlayerHUD)
	{
		PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());		
	}
	
	if (PlayerHUD)
	{
		PlayerHUD->EnemyDestroyed();
	}
}

EEnemyJumpResponse AEnemyBaseCharacter::PlayerJumpedOnEnemy()
{
	if (!MovementComponent->IsFalling())
	{
		if (EnemyJumpResponse == EEnemyJumpResponse::NoJump)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "player Jumped on Enemy");
			return EnemyJumpResponse;
		}
	
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, "PlayerJumpedOnEnemy()");
		DamageHandler->ForceEnemyStun();
		return EnemyJumpResponse;
	}

	return  EEnemyJumpResponse::NoJump;
}

// Called when the game starts or when spawned
void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnDestroyed.AddDynamic(this, &AEnemyBaseCharacter::OnActorDestroyed);

	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void AEnemyBaseCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	Super::FellOutOfWorld(dmgType);

	Destroy();
}

void AEnemyBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!MovementComponent)
	{
		MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
		MovementComponent->bUseControllerDesiredRotation = true;
	}

	if (BaseInfo)
	{
		EnemyJumpResponse = BaseInfo->JumpResponse;
		PickUpAmount = BaseInfo->PickUpAmount;
		PickUpType = BaseInfo->PickUpType;
		MovementComponent->MaxWalkSpeed = BaseInfo->MovementSpeed;
		DamageHandler->SetMaxHealth(BaseInfo->MaxHealth);
		DamageHandler->StunTime = BaseInfo->StunTime;

		UArmourContainerComponent *ArmourContainer = Cast<UArmourContainerComponent>(GetComponentByClass(UArmourContainerComponent::StaticClass()));
		if (ArmourContainer)
		{
			ArmourContainer->ArmourHealth = BaseInfo->ArmourMaxHealth;
			ArmourContainer->ArmourSettingsList = BaseInfo->ArmourSettings;
			// ArmourContainer->OnComponentDestroyed(false);
			ArmourContainer->OnComponentCreated();
		}
	}
}

// Called every frame
void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (!DamageHandler->GetIsFrozen())
	// {
	// 	GetMesh()->SetScalarParameterValueOnMaterials(FName("HeatPercent"), DamageHandler->GetHeatPercent());
	// } else
	// {
	// 	if (!IsHeatColorSwitched)
	// 	{
	// 		IsHeatColorSwitched = true;
	// 		GetMesh()->SetVectorParameterValueOnMaterials(FName("HeatColor"), FVector4(0.0f, 0.3f, 1.0f, 1.0f));
	// 		GetMesh()->SetScalarParameterValueOnMaterials(FName("HeatPercent"), 0.99f);
	// 	}
	// }
}

