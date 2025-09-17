// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerMovementStates/PlayerSlideState.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerMovementObjects/PlayerSlider.h"

void UPlayerSlideState::OnChange()
{
    if (Player->MovementObject)
    {       
        Player->MovementObject->SetActorTickEnabled(false);
        //Player->MovementObject = nullptr;
        Player->MovementObject->SetIsUsed(false);

        const float SaveYaw = Player->GetActorRotation().Yaw;

        //restore Player's rotation, except YAW
        Player->SetActorRotation(FRotator(0.0, SaveYaw, 0.0));
    }
}

void UPlayerSlideState::OnExecute()
{
    if (Player->MovementObject)
    {
        Super::OnExecute();
        Player->MovementManager->ResetJumps();
        Player->MovementManager->ResetDash();
        
        Player->MovementObject->SetActorTickEnabled(true);
        Player->PlayerMovementState = PlayerMovementStates::Slide;
    }
}
