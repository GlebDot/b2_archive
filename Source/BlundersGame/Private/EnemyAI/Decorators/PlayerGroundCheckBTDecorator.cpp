// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/EnemyAI/Decorators/PlayerGroundCheckBTDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UPlayerGroundCheckBTDecorator::UPlayerGroundCheckBTDecorator()
{
    NodeName = "Is Player On Ground";
}

bool UPlayerGroundCheckBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                               ::uint8* NodeMemory) const
{
    UBlackboardComponent *BB = OwnerComp.GetBlackboardComponent();
    ACharacter *Character = Cast<ACharacter>(BB->GetValueAsObject(Player.SelectedKeyName));

    if (Character)
    {
        return !Character->GetMovementComponent()->IsFalling();
    }

    return false;
}
