// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Tasks/PickAttackTargetBTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAI/EnemyParentAIController.h"
#include "Kismet/GameplayStatics.h"

UPickAttackTargetBTTaskNode::UPickAttackTargetBTTaskNode()
{
    NodeName = "Pick Attack Target";
}

EBTNodeResult::Type UPickAttackTargetBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyParentAIController *Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());

    if (Controller != nullptr && Controller->GetEnemyRef() != nullptr)
    {
        switch(TargetToSearch)
        {
            case EAttackTargetType::Player:
                FindPlayer();
                break;
            case EAttackTargetType::LevelController:
                break;
            default:
                break;
        }

        UBlackboardComponent *BlackboardComponent = Controller->GetBlackboardComponent();

        BlackboardComponent->SetValueAsObject(AttackTargetKey.SelectedKeyName, TargetActor);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

void UPickAttackTargetBTTaskNode::FindPlayer()
{
    TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}
