// Fill out your copyright notice in the Description page of Project Settings.

#include "BlundersGame/Public/EnemyAI/Tasks/PickEnemyTargetPointBTTaskNode.h"

#include "DrawDebugHelpers.h"
#include "BlundersGame/Public/EnemyAI/EnemyParentAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlundersGame/Public/EnemyAI/EnemyTargetPoint.h"

UPickEnemyTargetPointBTTaskNode::UPickEnemyTargetPointBTTaskNode()
{
    NodeName = "Pick Enemy Target Point";
}

EBTNodeResult::Type UPickEnemyTargetPointBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyParentAIController *Controller = Cast<AEnemyParentAIController>(OwnerComp.GetAIOwner());

    if (Controller && Controller->IsArenaMasterSet())
    {
        //Get BlackBoard
        UBlackboardComponent *BlackboardComponent = Controller->GetEnemyBlackboardComponent();

        //Get Current TargetPoint in use and Decrease it's weight
        AEnemyTargetPoint *CurrentTargetPoint = Cast<AEnemyTargetPoint>(BlackboardComponent->GetValueAsObject(TargetPointKey.SelectedKeyName));

        //Next Target point
        AEnemyTargetPoint *NextTargetPoint = nullptr;

        //Getting target points
        TArray<AEnemyTargetPoint*> Points = Controller->GetTargetPoints();

        switch (PickClassification)
        {
            case ETargetPointPickClassification::MaxHidden:
                Points.Sort([](const AEnemyTargetPoint& A, const AEnemyTargetPoint& B){
                    return A.GetHiddenFactor() > B.GetHiddenFactor();
                });
                break;
            case ETargetPointPickClassification::MinHidden:
                Points.Sort([](const AEnemyTargetPoint& A, const AEnemyTargetPoint& B){
                    return A.GetHiddenFactor() < B.GetHiddenFactor();
                });
                break;
            case ETargetPointPickClassification::MinWeight:
                Points.Sort([](const AEnemyTargetPoint& A, const AEnemyTargetPoint& B){
                    return A.GetTotalWeight() < B.GetTotalWeight();
                });
                break;
        }

        if (Points[0] != CurrentTargetPoint)
        {
            NextTargetPoint = Points[0];
        } else
        {
            NextTargetPoint = Points[1];
        }

        if (!NextTargetPoint)
        {
            return EBTNodeResult::Failed;
        }

        Controller->GetEnemyRef()->SetCurrentTargetPoint(NextTargetPoint);

        BlackboardComponent->SetValueAsObject(TargetPointKey.SelectedKeyName, NextTargetPoint);
        
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
