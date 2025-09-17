// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/GameManagment/PlatformPathKillerVolume.h"

#include "DamageComponents/DamageTypes/BlundersDamageType.h"
#include "GameFramework/GameModeBase.h"
#include "GameManagment/BlundersGameModeBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlatformPathKillerVolume::APlatformPathKillerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetCollisionProfileName("Trigger", true);
	if (BoxCollision)
	{
		SetRootComponent(BoxCollision);
	}

	DamageType = NewObject<UDamageType>();
}

void APlatformPathKillerVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlatformPathKillerVolume::OnBoxBeginOverlap);
}

void APlatformPathKillerVolume::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		ABlundersGameModeBase *GameMode = Cast<ABlundersGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->ResetLevel();
		}
	} else
	{
		if (OtherActor->GetClass()->IsChildOf(APawn::StaticClass()))
		{
			OtherActor->FellOutOfWorld(*DamageType);
			// UGameplayStatics::ApplyDamage(OtherActor, 100000.0f, nullptr, nullptr, UDamageType::StaticClass());
		}
	}
}

