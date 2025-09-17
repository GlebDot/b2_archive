// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Navigation/EnemyNavLauncher.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Enemies/EnemyParent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavLinkCustomComponent.h"
#include "EnemyAI/EnemyParentAIController.h"

// Sets default values
AEnemyNavLauncher::AEnemyNavLauncher()
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	if (Root)
	{
		SetRootComponent(Root);
	}
	
	Launcher = CreateDefaultSubobject<UBoxComponent>(TEXT("Launcher"));
	Launcher->SetupAttachment(Root);
	Launcher->bHiddenInGame = true;
	Launcher->SetBoxExtent(FVector(16.0f, 16.0f, 32.0f), true);
	Launcher->SetRelativeLocation(FVector::ZeroVector);

	NavLinkComponent = CreateDefaultSubobject<UNavLinkComponent>(TEXT("Nav Link Component"));
	NavLinkComponent->SetupAttachment(Root);

	TargetLocation = {100.0f, 0.0f, 0.0f};
	
	Launcher->OnComponentBeginOverlap.AddDynamic(this, &AEnemyNavLauncher::OnLauncherOverlap);
}

void AEnemyNavLauncher::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	NavLinkComponent->SetRelativeLocation(FVector::ZeroVector);

	// Assert that we haven't added or removed any Simple Links
	check(NavLinkComponent->Links.Num() == 1);

	// Setup link properties
	auto& Link = NavLinkComponent->Links[0];
	Link.Left = FVector::ZeroVector;
	Link.Right = TargetLocation;
	Link.Direction = ENavLinkDirection::LeftToRight;
}

void AEnemyNavLauncher::OnLauncherOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	AEnemyParent *Enemy = Cast<AEnemyParent>(OtherActor);
	if (Enemy)
	{
		UCapsuleComponent *Capsule = Cast<UCapsuleComponent>(OtherComp);
		if (Capsule == Enemy->GetCapsuleComponent())
		{
			AEnemyParentAIController *EnemyAIController = Cast<AEnemyParentAIController>(Enemy->GetController());
			const FVector CurrentPathPointLocation = EnemyAIController->GetPathFollowingComponent()->GetCurrentTargetLocation();
			if (CurrentPathPointLocation.Equals(GetActorLocation(), 16.0))
			{
				LaunchEnemy(Enemy);
				EnemyAIController->OnNavLauncherReach();
			}
		}
	}
}

void AEnemyNavLauncher::LaunchEnemy(AEnemyParent *Enemy)
{
	FVector LaunchVelocity;
	FVector Start = Enemy->GetActorLocation();
	Start.Z -= Enemy->GetSimpleCollisionHalfHeight();
	FVector End = GetActorTransform().TransformPosition(TargetLocation);
	End.Z += Enemy->GetSimpleCollisionHalfHeight();

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), LaunchVelocity, Start, End, 0, 0.5);
	// UGameplayStatics::SuggestProjectileVelocity(GetWorld(), LaunchVelocity, Start, End, 0.0f, false,
	// 	Enemy->GetSimpleCollisionHalfHeight(), 0, ESuggestProjVelocityTraceOption::TraceFullPath,
	// 	FCollisionResponseParams::DefaultResponseParam, TArray<AActor*>(), true);

	
	Enemy->LaunchCharacter(LaunchVelocity, true, true);
	const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
	Enemy->SetActorRotation(FRotator(0.0f, Rotation.Yaw, 0.0f), ETeleportType::TeleportPhysics);
}