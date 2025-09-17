#include "BlundersGame/Public/PlayerMovementStates/PlayerFallDownStates.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerBase/PlayerMovement/PlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerFallDownState::UPlayerFallDownState()
{
    StateType = EMovementStateSubtype::FallDown;
}

void UPlayerFallDownState::OnExecute()
{
    // Player->DisableInput(UGameplayStatics::GetPlayerController(Player->GetWorld(), 0));

    GravityScaleSave = MovementComponent->GravityScale;
    MovementComponent->GravityScale = Params.StateVelocity;
    Player->PlayerStop();

    Player->PlayerMovementState = PlayerMovementStates::FallDown;
    Super::OnExecute();
}

void UPlayerFallDownState::OnChange()
{
    // Player->EnableInput(UGameplayStatics::GetPlayerController(Player->GetWorld(), 0));
    if (MovementComponent->IsFalling())
    {
        Player->PlayerStop();
    }
    MovementComponent->GravityScale = GravityScaleSave;
    // OnFallDownComplete.Unbind();
}

void UPlayerFallDownState::OnLanding()
{
    Super::OnExecute();
    Player->ResetPlayerJumps();
    MovementComponent->GravityScale = GravityScaleSave;
    NotifyPlayerStateChange();
}
