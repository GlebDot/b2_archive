// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/Enemies/EnemyParent.h"
#include "BlundersGame/Public/DamageComponents/ArmourDamageHandlerComponent.h"
#include "DrawDebugHelpers.h"
#include "BlundersGame/Public/EnemyAI/EnemyParentAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BlundersGame/Public/Enemies/EnemyProjectiles/EnemyProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "BlundersGame/Public/Enemies/Misc/EnemyBaseInfoDataAsset.h"
#include "PlayerBase/UI/PlayerHUD.h"


void AEnemyParent::RegisterArena(AArenaMaster* Arena)
{
	AEnemyParentAIController *EnemyCon = Cast<AEnemyParentAIController>(GetController());
	if (EnemyCon)
	{
		EnemyCon->SetArenaRef(Arena);
	}
	ArenaMasterRef = Arena;
	ArenaMasterRef->AddWeight(EnemyWeight);
}

// Sets default values
AEnemyParent::AEnemyParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyParent::SetRotationLookAtTarget(float RotationInterpSpeed)
{
	if (CurrentTarget)
	{
		//calculate new rotation
		const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());

		const FRotator EnemyRotation = GetActorRotation();

		//calculate Yaw
		float TargetYawRotation = FMath::FInterpTo(EnemyRotation.Yaw, NewRotation.Yaw, GetWorld()->GetDeltaSeconds(), RotationInterpSpeed);			
		if (FMath::Sign(NewRotation.Yaw) != FMath::Sign(EnemyRotation.Yaw))
		{
			TargetYawRotation = NewRotation.Yaw;
		}
		
		//Set Yaw rotation 
		SetActorRotation(FRotator(0.0, TargetYawRotation, 0.0), ETeleportType::None);
	}
}

void AEnemyParent::Death_Implementation()
{
	
	ArenaMasterInfoCleanUp();

	Super::Death_Implementation();
}

// Called when the game starts or when spawned
void AEnemyParent::BeginPlay()
{
	Super::BeginPlay();
	CurrentTarget = PlayerRef;

	StopEffectDelegate = DamageHandler->OnStopEffectAchieved.AddUFunction(this, FName("OnStopEffect"));
}

void AEnemyParent::StunMontageEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_StunMontageEnd);

	GetMesh()->GetAnimInstance()->Montage_JumpToSection("StunEnd");
}

void AEnemyParent::OnStopEffect(float StunTime)
{
	AEnemyParentAIController *Con = Cast<AEnemyParentAIController>(GetController());
	if (Con && StunMontage)
	{
		DamageHandler->OnStopEffectAchieved.Remove(StopEffectDelegate);
		
		const int32 StunEndSectionIndex = StunMontage->GetSectionIndex("StunEnd");
		const float StunEndSectionLength = StunMontage->GetSectionLength(StunEndSectionIndex);
		PlayAnimMontage(StunMontage);
		GetWorld()->GetTimerManager().SetTimer(TH_StunMontageEnd, this, &AEnemyParent::StunMontageEnd, StunTime - StunEndSectionLength, false);

		Con->EnemyStun(StunTime);
	}
}

void AEnemyParent::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (BaseInfo)
	{
		EnemyWeight = BaseInfo->EnemyWeight;
	}
}

void AEnemyParent::RebindStopEffect()
{
	if (DamageHandler)
	{
		StopEffectDelegate = DamageHandler->OnStopEffectAchieved.AddUFunction(this, FName("OnStopEffect"));
	}
}

void AEnemyParent::SetCurrentTargetPoint(AEnemyTargetPoint* Point)
{
	if (CurrentTargetPoint)
	{
		CurrentTargetPoint->DecreaseEnemyWeight(this);
	}

	CurrentTargetPoint = Point;
	CurrentTargetPoint->AddEnemyWeight(this);
}

void AEnemyParent::ReceiveTickets(int Tickets)
{
	ArenaMasterRef->DecreaseTickets(Tickets);
	EnemyTickets += Tickets;
}

void AEnemyParent::ReturnTicketsToArena()
{
	if (EnemyTickets > 0 && ArenaMasterRef != nullptr)
	{
		ArenaMasterRef->AddTickets(EnemyTickets);
	}

	EnemyTickets = 0;
}

// Called every frame
void AEnemyParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyParent::Reset()
{
	Super::Reset();
	PickUpAmount = EPickUpAmount::Zero;
	Destroy();
}

void AEnemyParent::FellOutOfWorld(const UDamageType& dmgType)
{
	ArenaMasterInfoCleanUp();

	Super::FellOutOfWorld(dmgType);
}

void AEnemyParent::ArenaMasterInfoCleanUp()
{
	if (CurrentTargetPoint)
	{
		CurrentTargetPoint->DecreaseEnemyWeight(this);
	}

	if (ArenaMasterRef)
	{
		//Decrease Weight, because now we are dead 
		ArenaMasterRef->DecreaseWeight(EnemyWeight);

		//Always return tickets, what we currently have on hands
		ReturnTicketsToArena();
	
		//Leave Arena Master and erase itself from arena master list of enemies
		ArenaMasterRef->UnRegisterEnemy(this);
	}
}

