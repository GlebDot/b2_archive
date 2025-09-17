// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerMovementObjects/Teleport.h"

#include "Kismet/KismetMathLibrary.h"
#include "PlayerBase/PlayerCharacter.h"

// Sets default values
ATeleport::ATeleport()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}

	TeleportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportMesh"));
	TeleportMesh->SetupAttachment(SceneComponent);
	TeleportMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TeleportCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportCollision"));
	TeleportCollision->SetupAttachment(SceneComponent);
	TeleportCollision->SetCollisionProfileName("Trigger");

	TeleportColor = FLinearColor(0.0f, 0.45f, 1.0f, 1.0f);
}

FVector ATeleport::GetTeleportationLocation(const float TeleportActorHalfHeight) const
{
	return GetActorLocation() + GetActorForwardVector() * TeleportationLocationExtend + FVector(0.0f, 0.0f, TeleportActorHalfHeight);
}

void ATeleport::BeginPlay()
{
	Super::BeginPlay();

	if (TeleportTarget)
	{
		TeleportCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeleport::OnTeleportOverlap);
	}
}

void ATeleport::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FVector MaterialColor = FVector(TeleportColor.R, TeleportColor.G, TeleportColor.B);
	TeleportMesh->SetVectorParameterValueOnMaterials(FName("SecondaryColor"), MaterialColor);
	if (TeleportTarget)
	{
		TeleportTarget->TeleportMesh->SetVectorParameterValueOnMaterials(FName("SecondaryColor"), MaterialColor);
		TeleportTarget->SetTeleportsColor(TeleportColor);
	}
}

void ATeleport::OnTeleportOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		FVector SaveVelocity = PlayerCharacter->GetVelocity();
		PlayerCharacter->PlayerStop();
		PlayerCharacter->SpringArm->bEnableCameraLag = false;
		const FRotator PawnRotation = PlayerCharacter->GetActorRotation();
		const FRotator TeleportedRotation = TeleportTarget->GetActorForwardVector().Rotation();

		const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(TeleportedRotation, PawnRotation);
		
		const FVector TeleportedLocation = TeleportTarget->GetTeleportationLocation(OtherActor->GetSimpleCollisionHalfHeight());

		PlayerCharacter->SetActorLocation(TeleportedLocation, false, nullptr, ETeleportType::TeleportPhysics);
		float YawInputMultiplier = 1.0f;
		APlayerController *Con = Cast<APlayerController>(PlayerCharacter->GetController());
		if (Con)
		{
			YawInputMultiplier = Con->InputYawScale;
		}
		PlayerCharacter->AddControllerYawInput(DeltaRotator.Yaw / YawInputMultiplier);
		PlayerCharacter->SpringArm->bEnableCameraLag = true;

		SaveVelocity = SaveVelocity.RotateAngleAxis(DeltaRotator.Yaw, FVector(0.0f, 0.0f, 1.0f));
		PlayerCharacter->LaunchCharacter(SaveVelocity, true, true);
	}
}


