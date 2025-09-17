// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/Components/SpecialAbilities/SpecialAbilityDestroyer.h"

#include "PlayerBase/PlayerCharacter.h"
#include "PlayerBase/Components/SpecialAbilities/SpecialAbilityComponent.h"

// Sets default values
ASpecialAbilityDestroyer::ASpecialAbilityDestroyer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName("Trigger");
	if (BoxCollision)
	{
		SetRootComponent(BoxCollision);
	}

}

void ASpecialAbilityDestroyer::Destroyed()
{
	Super::Destroyed();

	BoxCollision->OnComponentBeginOverlap.Clear();
}

// Called when the game starts or when spawned
void ASpecialAbilityDestroyer::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpecialAbilityDestroyer::OnOverlap);
}

void ASpecialAbilityDestroyer::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		UActorComponent *Component = Player->GetComponentByClass(SpecialAbilityType);
		if (Component)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Special ability destroyed");
			Component->DestroyComponent();
		}
	} 
}

