#include "BlundersGame/Public/PlayerMovementStates/PlayerDashState.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerBase/PlayerMovement/PlayerMovementComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerMovement/MomentumAccumulatorComponent.h"

UPlayerDashState::UPlayerDashState()
{
    StateType = EMovementStateSubtype::Dash;
}

void UPlayerDashState::OnExecute()
{
    if (Player->MovementManager->DashCheck())
    {
        NotifyPlayerStateChange();
        const EPlayerMovementStates State = Player->AdvancedActionsComponent->EvaluateQueue();
        if (State != EPlayerMovementStates::Default)
        {
            Player->SetCurrentState(State);
            return;
        }
        BrakingFrictionSave = MovementComponent->BrakingFriction;
        BrakingFrictionFactorSave = MovementComponent->BrakingFrictionFactor;
        
        //If can dash (Have enough energy and not on cooldown)
        Player->PlayerMovementState = PlayerMovementStates::Dash;

        //Binding function to Timer Delegate
        TD_Dash.BindUFunction(Player, FName("PlayerStop"), MovementComponent->BrakingFrictionFactor, MovementComponent->BrakingFriction);

        //Set all friction to 0
        MovementComponent->BrakingFriction = 0.0;
        MovementComponent->BrakingFrictionFactor = 0.0;

        //Calculate Dash direction BASED ON PLAYER INPUT
        FVector DashDirection {};
        if (Player->GetMovementInputForward() == 0 && Player->GetMovementInputRight() == 0)
        {
            DashDirection = Player->GetActorForwardVector();
        } else
        {
            DashDirection = Player->GetActorForwardVector() * Player->GetMovementInputForward() + Player->GetActorRightVector() * Player->GetMovementInputRight();
        }
        DashDirection.Normalize();

        const float DashVelocity = CalculateMomentumAdjustedVelocity();
        MomentumComponent->ChangeDirectionPreserveMagnitude(DashDirection);
        Player->LaunchCharacter(DashDirection * DashVelocity, false, false);

        //Set timer to player stop
        Player->GetWorld()->GetTimerManager().SetTimer(TH_Dash, TD_Dash, Params.StateWorkTime, false);

        Super::OnExecute();
    }
}

void UPlayerDashState::OnChange()
{
    Super::OnChange();

    Player->GetWorld()->GetTimerManager().ClearTimer(TH_Dash);
    TD_Dash.Unbind();
    if(MovementComponent->BrakingFrictionFactor != BrakingFrictionFactorSave)
    {
        Player->PlayerStop(BrakingFrictionFactorSave, BrakingFrictionSave);
    }
}


UPlayerDashStateVertical::UPlayerDashStateVertical()
{
    StateType = EMovementStateSubtype::Empty;
}

void UPlayerDashStateVertical::OnExecute()
{
    Player->PlayerMovementState = PlayerMovementStates::Dash;

    BrakingFrictionSave = MovementComponent->BrakingFriction;
    BrakingFrictionFactorSave = MovementComponent->BrakingFrictionFactor;

    //Binding function to Timer Delegate
    TD_Dash.BindUFunction(Player, FName("PlayerStop"), MovementComponent->BrakingFrictionFactor, MovementComponent->BrakingFriction);

    //Set all friction to 0
    MovementComponent->BrakingFriction = 0.0;
    MovementComponent->BrakingFrictionFactor = 0.0;

    Player->PlayerStop();

    //Calculate Dash direction and Velocity 
    const FVector DashVelocity = Player->GetActorUpVector() * Params.StateVelocity;

    //Dash Launch
    Player->LaunchCharacter(DashVelocity, true, false);

    //Set timer to player stop
    Player->GetWorld()->GetTimerManager().SetTimer(
        TH_Dash,
        TD_Dash,
        Params.StateWorkTime,
        false);

    Super::OnExecute();
}

UPlayerDashStateLong::UPlayerDashStateLong()
{
    StateType = EMovementStateSubtype::Empty;
}

void UPlayerDashStateLong::OnExecute()
{
    Controller = Cast<APlayerController>(Player->GetController());
    Player->PlayerStop();
    Player->DisableInput(Controller);

    Player->PlayerMovementState = PlayerMovementStates::Dash;

    BrakingFrictionSave = MovementComponent->BrakingFriction;
    BrakingFrictionFactorSave = MovementComponent->BrakingFrictionFactor;

    MovementComponent->BrakingFriction = 0.0;
    MovementComponent->BrakingFrictionFactor = 0.0;

    const FVector DashDirection = Player->Camera->GetForwardVector();
    Player->LaunchCharacter(
        DashDirection * Params.StateVelocity,
        true,
        true);
    Player->GetWorld()->GetTimerManager().SetTimer(
        TH_Dash,
        this,
        &UPlayerDashStateLong::LongDashStop,
        Params.StateWorkTime,
        false);

    Super::OnExecute();
}

void UPlayerDashStateLong::LongDashStop()
{
    Player->PlayerStop(BrakingFrictionFactorSave, BrakingFrictionSave);
    Player->EnableInput(Controller);
}
