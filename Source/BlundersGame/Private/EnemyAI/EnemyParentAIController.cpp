// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/EnemyParentAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAI/EnemyPathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

void AEnemyParentAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // if (GetPathFollowingComponent() != nullptr)
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 0.0, FColor::Cyan, GetPathFollowingComponent()->GetName());
    //     uint32 Index = GetPathFollowingComponent()->GetCurrentPathIndex();
    //     FNavPathSharedPtr Path = GetPathFollowingComponent()->GetPath();
    //     if (Path.IsValid())
    //     {
    //         TArray<FNavPathPoint> Points = Path->GetPathPoints();
    //         for (int i = 0; i < Points.Num(); i++)
    //         {
    //             if (Index + 1 == i)
    //             {
    //                 DrawDebugSphere(GetWorld(), Points[Index + 1].Location, 50.0f, 12, FColor::Orange, false, 0.0);
    //             } else
    //             {
    //                 DrawDebugSphere(GetWorld(), Points[i].Location, 50.0f, 12, FColor::Green, false, 0.0);
    //             }
    //         }
    //     }      
    // } else
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Red, "NO PATHFOLLOWING");
    // }
}

AEnemyParentAIController::AEnemyParentAIController(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    EnemyBehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("EnemyBT"));
    EnemyBlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("EnemyBB"));

    GetPathFollowingComponent()->DestroyComponent();
    EnemyPFC = CreateDefaultSubobject<UEnemyPathFollowingComponent>(TEXT("EnemyPFC"));
    EnemyPFC->OnRequestFinished.AddUObject(this, &AEnemyParentAIController::OnMoveCompleted);

}

void AEnemyParentAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    EnemyRef = Cast<AEnemyParent>(InPawn);

    if (EnemyRef)
    {
        if (EnemyRef->EnemyBehaviourTree->BlackboardAsset)
        {
            EnemyBlackboardComponent->InitializeBlackboard(*(EnemyRef->EnemyBehaviourTree->BlackboardAsset));
        }

        EnemyBehaviourTreeComponent->StartTree(*(EnemyRef->EnemyBehaviourTree));
    }
    
    if (EnemyRef)
    {
        EnemyRef->LandedDelegate.AddUniqueDynamic(this, &AEnemyParentAIController::OnLanded);
        EnemyRef->MovementModeChangedDelegate.AddUniqueDynamic(this, &AEnemyParentAIController::OnMovementModeChanged);
    }

    GetPathFollowingComponent()->SetPreciseReachThreshold(2.0, 2.0);
}

void AEnemyParentAIController::OnLanded(const FHitResult& HitInfo)
{
    GetPathFollowingComponent()->ResumeMove();
    GetBrainComponent()->ResumeLogic(FString(""));
    UCharacterMovementComponent *MovementComponent = Cast<UCharacterMovementComponent>(GetCharacter()->GetMovementComponent());
    if (MovementComponent)
    {
        MovementComponent->bOrientRotationToMovement = true;
    }
}

void AEnemyParentAIController::OnMovementModeChanged(ACharacter* MovedCharacter, EMovementMode PrevMovementMode,
    uint8 PreviousCustomMode)
{
    if (MovedCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
    {
        GetPathFollowingComponent()->PauseMove(FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Reset);
        GetBrainComponent()->PauseLogic(FString(""));
        
        UCharacterMovementComponent *MovementComponent = Cast<UCharacterMovementComponent>(MovedCharacter->GetMovementComponent());
        if (MovementComponent)
        {
            MovementComponent->bOrientRotationToMovement = true;
            const FRotator VelocityRotation = MovedCharacter->GetVelocity().Rotation();
            SetControlRotation(FRotator(0.0f, VelocityRotation.Yaw, 0.0f));
            MovedCharacter->SetActorRotation(FRotator(0.0f, VelocityRotation.Yaw,0.0f), ETeleportType::None);
        }
    }
}

void AEnemyParentAIController::InitPlayerBBKey()
{
    EnemyBlackboardComponent->SetValueAsObject(PlayerKeyName, PlayerRef);
}

void AEnemyParentAIController::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    SetPathFollowingComponent(EnemyPFC);
}

TArray<AEnemyTargetPoint*> &AEnemyParentAIController::GetTargetPoints() const
{
    return ArenaMasterRef->GetTargetPoints();
}

void AEnemyParentAIController::SetPlayerRef()
{
    if (!PlayerRef)
    {
        PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        SetFocus(PlayerRef);
        // if (!PlayerRef)
        // {
        //     GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Purple, "No Player");
        // }
    }
}

void AEnemyParentAIController::EnemyStun(float StunTime)
{
    //Stop tree logic, enemy do nothing
    EnemyBehaviourTreeComponent->StopTree();
    EnemyRef->ReturnTicketsToArena();

    //Set timer to reset enemy logic
    GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &AEnemyParentAIController::EnemyStunOut, StunTime, false);
}

void AEnemyParentAIController::OnNavLauncherReach()
{
    EnemyPFC->OnNavLaunch();
}

void AEnemyParentAIController::EnemyStunOut()
{
    EnemyBehaviourTreeComponent->StartTree(*EnemyRef->EnemyBehaviourTree);

    // if (EnemyRef)
    // {
    //     EnemyRef->RebindStopEffect();
    // }    
}

void AEnemyParentAIController::Destroyed()
{
    Super::Destroyed();

    if (EnemyRef)
    {
        EnemyRef->LandedDelegate.Remove(this, FName("OnLanded"));
        EnemyRef->MovementModeChangedDelegate.Remove(this, FName("OnMovementModeChanged"));
    }
}

void AEnemyParentAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
}

void AEnemyParentAIController::BeginPlay()
{
    Super::BeginPlay();

    SetPlayerRef();
   
    FTimerHandle InitBBKeyTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(InitBBKeyTimerHandle, this, &AEnemyParentAIController::InitPlayerBBKey, 0.5, false);
}
