#include "BlundersGame/Public/PlayerMovementStates/PlayerJumpStates.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerBase/MovementManager.h"
#include "BlundersGame/Public/PlayerBase/Components/AdvancedActionsComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerMovement/PlayerMovementComponent.h"
#include "BlundersGame/Public/PlayerBase/PlayerMovement/MomentumAccumulatorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////SLIDER//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


UPlayerJumpStateSlider::UPlayerJumpStateSlider()
{
    StateType = EMovementStateSubtype::SlideJump;
}

void UPlayerJumpStateSlider::OnExecute()
{
    NotifyPlayerStateChange();
    FVector LaunchVelocity = Player->GetActorForwardVector() * Params.StateVelocity;
    LaunchVelocity.Z = Params.StateVelocity / StateMultiplier;

    Player->LaunchCharacter(LaunchVelocity, false, true);
    Player->PlayerMovementState = PlayerMovementStates::Jump;
    Super::OnExecute();
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////GROUND//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

UPlayerJumpStateGround::UPlayerJumpStateGround()
{
    StateType = EMovementStateSubtype::Jump;
}

void UPlayerJumpStateGround::OnExecute()
{
    //TODO: why this ifs are here?
    if (Player->MovementManager->JumpCheck(JumpTypes::Ground))
    {
        NotifyPlayerStateChange();
        const EPlayerMovementStates State = Player->AdvancedActionsComponent->EvaluateQueue();
        if (State != EPlayerMovementStates::Default)
        {
            Player->SetCurrentState(State);
            return;
        }
        const FVector JumpVelocity = FVector(0.0f, 0.0f, Params.StateVelocity);
        Player->LaunchCharacter(JumpVelocity, false, true);
        Player->PlayerMovementState = PlayerMovementStates::Jump;

        Super::OnExecute();
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Enemy /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//TODO: WTF? this simple jumps are just the same code?
UPlayerJumpStateEnemy::UPlayerJumpStateEnemy()
{
    StateType = EMovementStateSubtype::Jump;
}

void UPlayerJumpStateEnemy::OnExecute()
{
    NotifyPlayerStateChange();
    const EPlayerMovementStates State = Player->AdvancedActionsComponent->EvaluateQueue();
    if (State != EPlayerMovementStates::Default)
    {
        Player->SetCurrentState(State);
        return;
    }
    const FVector JumpVelocity = FVector(0.0f, 0.0f, Params.StateVelocity);
    Player->LaunchCharacter(JumpVelocity, false, true);
    Player->PlayerMovementState = PlayerMovementStates::Jump;
    Super::OnExecute();
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////MID AIR/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
UPlayerJumpStateMidAir::UPlayerJumpStateMidAir()
{
    StateType = EMovementStateSubtype::Jump;
}

void UPlayerJumpStateMidAir::OnExecute()
{
    if (Player->MovementManager->JumpCheck(JumpTypes::Air))
    {
        NotifyPlayerStateChange();
        const EPlayerMovementStates State = Player->AdvancedActionsComponent->EvaluateQueue();
        if (State != EPlayerMovementStates::Default)
        {
            Player->SetCurrentState(State);
            return;
        }
        const FVector JumpVelocity = FVector(0.0f, 0.0f, Params.StateVelocity);
        Player->LaunchCharacter(JumpVelocity, false, true);
        Player->PlayerMovementState = PlayerMovementStates::Jump;

        Super::OnExecute();
    }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////WALL////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
UPlayerJumpStateWall::UPlayerJumpStateWall()
{
    StateType = EMovementStateSubtype::WallJump;
}

void UPlayerJumpStateWall::OnExecute()
{
    if (!Controller)
    {
        Controller = Cast<APlayerController>(Player->GetController());
    }
    
    if (Player->MovementManager->JumpCheck(JumpTypes::Wall))
    {
        GravityScaleSave = MovementComponent->GravityScale;
        MovementComponent->GravityScale = 0.0f;
        MovementComponent->StopMovementImmediately();
        Player->DisableInput(Controller);

        if ( Params.StateWorkTime > KINDA_SMALL_NUMBER)
        {
            Player->GetWorld()->GetTimerManager().SetTimer(TH_LaterJump, this, &UPlayerJumpStateWall::LaterJump, Params.StateWorkTime, false);
        }
        else
        {
            LaterJump();
        }

        Super::OnExecute();
    }
}

void UPlayerJumpStateWall::LaterJump()
{
    MovementComponent->GravityScale = GravityScaleSave;
    Player->GetWorld()->GetTimerManager().ClearTimer(TH_LaterJump);
    const FVector JumpVelocity = FVector(0.0f, 0.0f, Params.StateVelocity);
    Player->LaunchCharacter(JumpVelocity, true, true);
    Player->EnableInput(Controller);
    Player->PlayerMovementState = PlayerMovementStates::Jump;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////LONG////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
UPlayerJumpStateLong::UPlayerJumpStateLong()
{
    StateType = EMovementStateSubtype::Empty;
}

void UPlayerJumpStateLong::OnExecute()
{
    FVector LaunchVector {};
    const FVector StartLocation = Player->GetActorLocation();
    const FVector EndLocation = StartLocation + Player->GetActorForwardVector() * CalculateMomentumAdjustedVelocity();
    UGameplayStatics::SuggestProjectileVelocity_CustomArc(
        Player->GetWorld(),
        LaunchVector,
        StartLocation,
        EndLocation,
        0,
        Params.MovementArcAngle);

    Player->LaunchCharacter(LaunchVector, false, true);
    Player->PlayerMovementState = PlayerMovementStates::Jump;

    Super::OnExecute();
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////BACK////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
UPlayerJumpStateBackward::UPlayerJumpStateBackward()
{
    StateType = EMovementStateSubtype::Empty;
}

void UPlayerJumpStateBackward::OnExecute()
{
    Controller = Cast<APlayerController>(Player->GetController());
    Player->PlayerStop();
    Player->DisableInput(Controller);
    StartRotation = Player->GetActorRotation();
    Player->GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &UPlayerJumpStateBackward::RotateCharacter, 0.005, true);
    RotationRate = Params.StateWorkTime / 180.0f;
    
        
    FVector LaunchVector;
    const FVector StartLocation = Player->GetActorLocation();
    const FVector EndLocation = StartLocation + Player->GetActorForwardVector() * -Params.StateVelocity;
    UGameplayStatics::SuggestProjectileVelocity_CustomArc(
        Player->GetWorld(),
        LaunchVector,
        StartLocation,
        EndLocation,
        0,
        Params.MovementArcAngle);

    Player->LaunchCharacter(LaunchVector, false, true);
    Player->PlayerMovementState = PlayerMovementStates::Jump;

    Super::OnExecute();
}

void UPlayerJumpStateBackward::RotateCharacter()
{
    if (Player)
    {
        Player->AddControllerYawInput(RotationRate);
        
        if (UKismetMathLibrary::NormalizedDeltaRotator(Player->GetActorRotation(), StartRotation).Yaw >= 160.0f)
        {
            Player->EnableInput(Controller);
            Player->GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////JUMPER//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
UPlayerJumpStateJumper::UPlayerJumpStateJumper()
{
    StateType = EMovementStateSubtype::Jump;
}

void UPlayerJumpStateJumper::OnExecute()
{    
    Player->LaunchCharacter(JumpVelocity, true, true);
    Player->PlayerMovementState = PlayerMovementStates::Jump;

    Super::OnExecute();
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////HIGH////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
UPlayerJumpStateHigh::UPlayerJumpStateHigh()
{
    StateType = EMovementStateSubtype::Empty;
}

void UPlayerJumpStateHigh::OnExecute()
{
    Player->PlayerStop();
    const FVector JumpVelocity = FVector(0.0f, 0.0f, Params.StateVelocity);
    Player->LaunchCharacter(JumpVelocity, true, true);
    Player->PlayerMovementState = PlayerMovementStates::Jump;

    Super::OnExecute();
}

