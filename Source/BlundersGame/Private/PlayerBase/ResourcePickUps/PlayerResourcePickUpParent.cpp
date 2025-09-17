// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase/ResourcePickUps//PlayerResourcePickUpParent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase/PlayerCharacter.h"

// Sets default values
APlayerResourcePickUpParent::APlayerResourcePickUpParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetCollisionProfileName("Trigger", true);
	SphereCollision->SetSphereRadius(175.0f, true);
	
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickUpMesh"));
	PickUpMesh->SetCollisionProfileName("PickUp", true);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/PlayerPickUps/Meshes/PickUp.PickUp'"));
	if (Mesh.Succeeded())
	{
		PickUpMesh->SetStaticMesh(Mesh.Object);
	}

	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	if (PickUpMesh)
	{
		SetRootComponent(PickUpMesh);
	}
	SphereCollision->SetupAttachment(PickUpMesh);
}

void APlayerResourcePickUpParent::Reset()
{
	Super::Reset();
	Destroy();
}

void APlayerResourcePickUpParent::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerResourcePickUpParent::OnSphereOverlapBegin);

	GetWorld()->GetTimerManager().SetTimer(TH_OverlapCheckSpawn, this, &APlayerResourcePickUpParent::OverlapCheckOnSpawn, 1.0f, false);
}

void APlayerResourcePickUpParent::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		const bool Res = PlayerPickCheck(Player);
		if (Res)
		{
			Destroy();
		}
	}
}

bool APlayerResourcePickUpParent::PlayerPickCheck(AActor* OtherActor)
{
	APlayerCharacter *Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		TransferResources(Player);
		return true;
	}

	return false;
}

void APlayerResourcePickUpParent::OverlapCheckOnSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_OverlapCheckSpawn);
	TArray<FOverlapResult> OverlapResults;
	SphereCollision->ComponentOverlapMulti(OverlapResults, GetWorld(), GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Pawn);
	for (auto &OverlapResult : OverlapResults)
	{
		const bool Res = PlayerPickCheck(OverlapResult.GetActor());
		if (Res)
		{
			Destroy();
			return;
		}
	}
}


